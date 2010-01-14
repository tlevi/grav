#include "Physics.h"
#include <cstdlib>


#define G 6.67e-11


vector<physobj> Physics::objs;
int Physics::objcount;
bool Physics::objschanged;
float Physics::td = 1e6;
float Physics::tdsqr = td*td;
vector2 Physics::boxmax(100, 100);
vector2 Physics::boxmin(0, 0);
vector2 Physics::boxsz(boxmax-boxmin);


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
			const physobj& b = objs[k];

			const vector2 v = (b.p - a.p);
			const float dist = LENGTH(v);
			const float gravfactor =  G * b.mass / (dist*dist*dist);

			aa += v * gravfactor;
		}

		a.a = aa;
	}
};


const void Physics::addObject(const float radius){
	physobj newobj(radius);
	newobj.p = vector2((rand()/RAND_MAX), (rand()/RAND_MAX)) * boxsz + boxmin;
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

