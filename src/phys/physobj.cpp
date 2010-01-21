#include "physobj.h"
#include "../shared.h"

typedef unsigned int Uint32;


unsigned int physobj::nextid = 1;


physobj::physobj(const real aRadius) : id(nextid++){
	radius = aRadius;
	mass = radius * radius * M_PI * 4.0;
	p = vector2(0, 0);

	const Uint32 r = Uint32(64+FRAND*191) & 0xff;
	const Uint32 g = Uint32(64+FRAND*191) & 0xff;
	const Uint32 b = Uint32(64+FRAND*191) & 0xff;
	color = (r<<16) | (g<<8) | b;
};
