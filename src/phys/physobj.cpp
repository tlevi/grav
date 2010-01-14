#include "physobj.h"


physobj::physobj(const float aRadius){
	radius = aRadius;
	mass = radius * radius * M_PI;
	p = vector2(0.0f, 0.0f);
	po = vector2(0.0f, 0.0f);
	a = vector2(0.0f, 0.0f);
	invmass = 1.0 / mass;
};


physobj::~physobj(){
};
