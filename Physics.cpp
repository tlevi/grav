#include "Physics.h"


vector<physobj> Physics::objs;
int Physics::objcount;
bool Physics::objschanged;
float Physics::tdsqr;
float Physics::tdr;
float Physics::td;


const void Physics::updatePosition(){
	const int max = objcount;
	for (int i=0; i < max; i++){
		physobj& o = objs[i];
		const vector2 po = o.p;
		o.p += (o.p - o.po) * tdr + o.a * tdsqr;
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
//			physobj& b = objs[k];
			//TODO
		}

		a.a = aa;
	}
};


const void Physics::addObject(const physobj& newobj){
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

	objs.erase(objs.begin()+idx);

	objcount = objs.size();
	objschanged = true;
};


const void Physics::changeTimeScale(const float factor){
	//TODO
};
