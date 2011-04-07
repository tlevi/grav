#ifndef _OBJECT_H_
#define _OBJECT_H_


#include "physic.h"
#include "shared.h"


#define MAX_OBJECTS 8192


void makeObjects(void);
void updateObjects(void);

class object{
public:
	static const void updateObjects(void);
	static const void makeObjects(void);

	object(void);
	~object(void);
	physics * physic;
	SDL_Surface * pSurf;
	struct{
		short x;
		short y;
		short ox;
		short oy;
		uInt color;
		uInt tcolor;
	} display;
	short tpx;
	short tpy;
};


extern object * pObjects[MAX_OBJECTS];


#endif
