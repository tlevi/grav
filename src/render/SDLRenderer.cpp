#ifdef API_SDL
#include "SDLRenderer.h"
#include <iostream>
#include <cmath>

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
	SDL_WM_SetCaption("Grav", NULL);
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
	int scale = width/100;

	for(unsigned int i = 0; i < objs.size(); i++) {
		int scaleRadius = objs[i].get_radius()*scale;
		for(int y = 1-scaleRadius; y < scaleRadius; y++) {
			int dist = sqrt(scaleRadius*scaleRadius - y*y);
			for(int x = -dist; x <= dist; x++) {
				int pixelX = int(objs[i].get_p().x)*width/100 + x;
				int pixelY = int(objs[i].get_p().y)*height/100 + y;
				if(pixelX > 0 && pixelX < width && pixelY > 0 && pixelY < height) *(Uint32 *)((Uint8 *)pPixelsScreen + pixelY*pSurfScreen->pitch + pixelX*4) = 0xFFFFFFFF;
			}
		}
	}

	endFrame();
};


const void SDLRenderer::updateTrails(const vector<physobj>& objs){
	//TODO SDLRenderer::updateTrails()
};


#endif /* API_SDL */
