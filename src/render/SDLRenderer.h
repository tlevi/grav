#ifndef SDLRENDERER_H_
#define SDLRENDERER_H_

#include "Renderer.h"


class SDLRenderer: public Renderer{
public:
	SDLRenderer();
	~SDLRenderer();


	const void drawObjects(const vector<physobj>& objs);

	const void updateTrails(const vector<physobj>& objs);
	const void fadeTrails();
	const void clearTrails();

	const bool requestScreen(short width, short height, bool fullScreen);
};


#endif /* SDLRENDERER_H_ */
