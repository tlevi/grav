#ifdef API_SDL
#include "SDLRenderer.h"
#include <iostream>


SDLRenderer::SDLRenderer(SDLInput& sdlinput){
	pPixelsScreen = NULL;
	pPixelsTrails = NULL;
	pSurfScreen = NULL;
	pSurfTrails = NULL;
};


SDLRenderer::~SDLRenderer(){
	SDL_FreeSurface(pSurfTrails);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
};


const bool SDLRenderer::requestScreen(){
	if (pSurfTrails != NULL) SDL_FreeSurface(pSurfTrails);

	const Uint32 fsflag = (fullscreen) ? SDL_FULLSCREEN : 0;
	pSurfScreen = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_ASYNCBLIT|fsflag);
	if (pSurfScreen == NULL) return false;

	const SDL_PixelFormat& pf = *pSurfScreen->format;
	pSurfTrails = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, pf.Rmask, pf.Gmask, pf.Bmask, pf.Amask);

	return true;
};


const void SDLRenderer::startFrame(){
	if (SDL_MUSTLOCK(pSurfScreen)) SDL_LockSurface(pSurfScreen);
	if (SDL_MUSTLOCK(pSurfTrails)) SDL_LockSurface(pSurfTrails);
	pPixelsScreen = pSurfScreen->pixels;
	pPixelsTrails = pSurfTrails->pixels;
};


const void SDLRenderer::endFrame(){
	if (SDL_MUSTLOCK(pSurfScreen)) SDL_UnlockSurface(pSurfScreen);
	if (SDL_MUSTLOCK(pSurfTrails)) SDL_UnlockSurface(pSurfTrails);
	pPixelsScreen = NULL;
	pPixelsTrails = NULL;
	SDL_Flip(pSurfScreen);
};


const void SDLRenderer::doDrawing(const vector<physobj>& objs){
	startFrame();
	endFrame();
};


const void SDLRenderer::updateTrails(const vector<physobj>& objs){
	//TODO SDLRenderer::updateTrails()
};


#endif /* API_SDL */
