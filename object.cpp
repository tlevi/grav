#include "object.h"
#include "main.h"
#include "time.h"
#include "render.h"


object * pObjects[MAX_OBJECTS];


object::object(void){
	tpx = -1;
	tpy = -1;

	if (Global::objectcount > MAX_OBJECTS){
		fprintf(stderr, "Too many objects (MAX_OBJECTS), %u, %u\n", Global::objectcount, MAX_OBJECTS);
		exit(1);
	}
	pObjects[Global::objectcount] = this;
	Global::objectcount++;

	if (Global::bRandomMass == true){
		physic = new physics(128+FRAND*1800);
	}
	else physic = new physics(256);

	physic->np.x = (FRAND - 0.5f) * float(Global::screenWidth);
	physic->np.y = (FRAND - 0.5f) * float(Global::screenHeight);
	physic->op = physic->np;
	display.color = SDL_MapRGB(Global::buffer->format, (uChar)(64+FRAND*191), (uChar)(64+FRAND*191), (uChar)(64+FRAND*191));

	const uChar tr = (display.color & 0x00ff0000) >> 17;
	const uChar tg = (display.color & 0x0000ff00) >> 9;
	const uChar tb = (display.color & 0x000000ff) >> 1;
	display.tcolor = (((tr << 8) | tg) << 8) | tb;

	display.ox = display.x = short(physic->np.x + Global::screenWidth/2);
	display.oy = display.y = short(Global::screenHeight/2 - physic->np.y);

	SDL_PixelFormat * sf = Global::screen->format;
	const int r = (int)(physic->radius*2 + 1);
	pSurf = SDL_CreateRGBSurface(SDL_SWSURFACE, r, r, sf->BitsPerPixel, sf->Rmask, sf->Gmask, sf->Bmask, sf->Amask);
	SDL_SetAlpha(pSurf, SDL_SRCALPHA, SDL_ALPHA_OPAQUE);
	SDL_SetColorKey(pSurf, SDL_SRCCOLORKEY, SDL_MapRGB(pSurf->format, 0, 0, 0));
	drawCircle_Surface(this);
	return;
}


//Free other memory held by object (physic)
//Find pointer in objects array
//Move elements backwards one (yes, O(n), thats life)
object::~object(void){
	delete this->physic;
	this->physic = NULL;
	SDL_FreeSurface(pSurf);
	uShort i = 0;
	for (; i < Global::objectcount; i++){
		if (pObjects[i]==this) break;
	}
	for (i++; i < Global::objectcount; i++){
		pObjects[i-1] = pObjects[i];
	}
	Global::objectcount--;
}


const void object::updateObjects(void){
	if (Global::objectcount == 0) return;

	for (uShort i=0; i < Global::objectcount; i++){
		object * pObj = pObjects[i];
		const physics * pPhys = pObj->physic;

		pObj->display.ox = pObj->display.x;
		const float t1 = pPhys->np.x + float(Global::screenWidth)*0.5;
		if (t1 > 0 && t1 < Global::screenWidth)	pObj->display.x = t1;
		else pObj->display.x = Global::screenWidth/2;

		pObj->display.oy = pObj->display.y;
		const float t2 = float(Global::screenHeight)*0.5 - pPhys->np.y;
		if (t2 > 0 && t2 < Global::screenHeight) pObj->display.y = t2;
		else pObj->display.y = Global::screenHeight/2;
	}

	return;
}


//Create initial set
//Make sure it is stable, solve constraints
//Finally, objects start at rest
const void object::makeObjects(void){
	if (Global::maxobjects == 0) return;
	//make objects
	for (uShort i=0; i < Global::maxobjects; i++) new object();
	//loop constraints for a while to put objects outside each other
	for (int i=100; i>0; i--) physics::satisfyConstraints();
	//initial velocities zero (in case constraints gave it a velocity)
	for (int i=0; i < Global::maxobjects; i++) pObjects[i]->physic->clearVelocity();
	return;
}
