#ifdef API_SDL
#ifndef SDLRENDERER_H_
#define SDLRENDERER_H_

#include "Renderer.h"
#include "../input/SDLInput.h"


class SDLRenderer: public Renderer{
private:
	void* pPixelsScreen;
	void* pPixelsTrails;
	SDL_Surface* pSurfScreen;
	SDL_Surface* pSurfTrails;

	const void startFrame();
	const void drawObjects(const vector<physobj>& objs);
	const void updateTrails(const vector<physobj>& objs);
	const void fadeTrails();
	const void clearTrails();
	const void endFrame();

public:
	SDLRenderer(SDLInput& sdlinput);
	~SDLRenderer();

	const void doDrawing(const vector<physobj>& objs);
	const bool requestScreen();
};


#endif /* SDLRENDERER_H_ */
#endif /* API_SDL */
