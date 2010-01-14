#include "physobj.h"


physobj::physobj(){
	//TODO: physobj()
};


physobj::physobj(const float radius){
	this->radius = radius;
	mass = radius * radius * M_PI;
	p = vector2(0.0f, 0.0f);
	po = vector2(0.0f, 0.0f);
	a = vector2(0.0f, 0.0f);
	invmass = 1.0 / mass;
};


physobj::~physobj(){
};
