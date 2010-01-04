#ifndef RENDERER_H_
#define RENDERER_H_

#include <vector>
#include "physobj.h"


/* Just an interface */
class Renderer{
protected:
	Renderer();


public:
	virtual ~Renderer() = 0;

	virtual const void drawObjects(const vector<physobj>& objs) = 0;

	virtual const void updateTrails(const vector<physobj>& objs) = 0;
	virtual const void fadeTrails() = 0;
	virtual const void clearTrails() = 0;

	virtual const bool requestScreen(short width, short height, bool fullScreen) = 0;
};


#endif /* RENDERER_H_ */
