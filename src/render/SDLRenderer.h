#ifdef API_SDL
#ifndef SDLRENDERER_H_
#define SDLRENDERER_H_

#include "Renderer.h"
#include "../input/SDLInput.h"


class SDLRenderer: public Renderer{
public:
	SDLRenderer(SDLInput& sdlinput);
	~SDLRenderer();


	const void startFrame();
	const void drawObjects(const vector<physobj>& objs);
	const void updateTrails(const vector<physobj>& objs);
	const void fadeTrails();
	const void clearTrails();
	const void endFrame();

	const bool requestScreen();
};


#endif /* SDLRENDERER_H_ */
#endif /* API_SDL */
