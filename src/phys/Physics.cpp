#include "Physics.h"
#include "../shared.h"
#include "../Config.h"


#define G (6.67e-11)


vector<physobj> Physics::objs;
int Physics::objcount;
bool Physics::objschanged;
float Physics::td = 1e2 * 4.0;
float Physics::tdsqr = td*td;
vector2 Physics::boxsz(100, 100);
vector2 Physics::boxmin(0, 0);
vector2 Physics::boxmax(boxmin+boxsz);


const void Physics::Initialise(){
	const int width = atoi(Config::get("r_width").c_str());
	const int height = atoi(Config::get("r_height").c_str());

	if (width != 0 && height != 0){
		boxsz.x = sqrtf(boxsz.x * boxsz.y * width / height);
		boxsz.y = boxsz.x * height / width;

		cout << "Physics: physbox dimensions are " << boxsz.x << ", " << boxsz.y << endl;
		boxmax = boxmin + boxsz;
	}

	const int maxobjs = atoi(Config::get("objs").c_str());
	const float maxradius = 4.0;
	const float minradius = 1.0;

	for (int i=0; i < maxobjs; i++){
		Physics::addObject(minradius + FRAND * (maxradius-minradius));
	}
};


const void Physics::updatePosition(){
	const int max = objcount;
	for (int i=0; i < max; i++){
		physobj& o = objs[i];
		const vector2 po = o.p;
		o.p += (o.p - o.po) + o.a * tdsqr;
		o.po = po;
	}
};


const void Physics::updateAcceleration(){
	const int max = objcount;

	#pragma omp parallel for schedule(static)
	for (int i=0; i < max; i++){
		physobj& a = objs[i];
		vector2 aa(0.0f, 0.0f);

		for (int k=0; k < max; k++){
			if (k == i) continue;
			const physobj& b = objs[k];

			const vector2 v = (b.p - a.p);
			const float dist = LENGTH(v);
			if (dist < 1.0) continue; //BUGBUG, just fixes crazy accel for now
			const float gravfactor =  G * b.mass / (dist*dist*dist);

			aa += v * gravfactor;
		}

		a.a = aa;
	}
};


const void Physics::addObject(const float radius){
	physobj newobj(radius);
	newobj.p = vector2(FRAND, FRAND) * (boxsz - vector2(radius*2, radius*2)) + boxmin + vector2(radius, radius);
	newobj.po = newobj.p;

	objs.push_back(newobj);
	objcount = objs.size();
	objschanged = true;
};


const void Physics::delObject(const physobj& oldobj){
	const int max = objcount;
	int idx = 0;

	while (idx < max){
		if (&oldobj == &objs[idx]) break;
		else idx++;
	}

	delObject(idx);
};


const void Physics::delObject(const int idx){
	objs.erase(objs.begin()+idx);
	objcount = objs.size();
	objschanged = true;
};


const void Physics::changeTimeScale(const float factor){
	td *= factor;
	tdsqr = td*td;

	const int max = objcount;
	for (int i=0; i < max; i++){
		physobj& obj = objs[i];

		obj.po = obj.p + (obj.po - obj.p) * factor;
	}
};


const void Physics::advanceTick(){
	updateAcceleration();
	updatePosition();
	//TODO: finish Physics::advanceTick()
};

