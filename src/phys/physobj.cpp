#include "physobj.h"


unsigned int physobj::nextid = 1;


physobj::physobj(const float aRadius) : id(nextid++){
	radius = aRadius;
	mass = radius * radius * M_PI;
	p = vector2(0.0f, 0.0f);
	po = vector2(0.0f, 0.0f);
	a = vector2(0.0f, 0.0f);
	invmass = 1.0 / mass;
};
