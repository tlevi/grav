#ifdef API_SDL
#include <iostream>
#include <cmath>
#include "SDLRenderer.h"
#include "../shared.h"
#include "../phys/Physics.h"


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
	pPixelsScreen = static_cast<Uint32*>(pSurfScreen->pixels);
	pPixelsTrails = static_cast<Uint32*>(pSurfTrails->pixels);
};


const void SDLRenderer::endFrame(){
	if (SDL_MUSTLOCK(pSurfScreen)) SDL_UnlockSurface(pSurfScreen);
	if (SDL_MUSTLOCK(pSurfTrails)) SDL_UnlockSurface(pSurfTrails);
	pPixelsScreen = NULL;
	pPixelsTrails = NULL;
	SDL_Flip(pSurfScreen);
};


const void SDLRenderer::drawObjects(const vector<physobj>& objs){
	const float scale = width / Physics::getBBmax().x;
	const int pitch = pSurfScreen->pitch / 4;

	for(unsigned int i = 0; i < objs.size(); i++) {
		const int scaleRadius = objs[i].get_radius()*scale;
		const vector2& p = objs[i].get_p() * scale;
		const int px = int(p.x + 0.5);
		const int py = int(p.y + 0.5);

		for(int y = -scaleRadius; y <= scaleRadius; y++){
			const int dist = sqrtf(scaleRadius*scaleRadius - y*y);
			const int pixelY = py + y;
			if (pixelY >= height) break;
			if (pixelY < 0) continue;
			for(int x = -dist; x <= dist; x++){
				const int pixelX = px + x;
				if (pixelX >= width) break;
				if (pixelX < 0) continue;
				pPixelsScreen[pixelY*pitch + pixelX] = objs[i].get_color();
			}
		}
	}
};


const void SDLRenderer::doDrawing(const vector<physobj>& objs){
	startFrame();

	SDL_FillRect(pSurfScreen, NULL, 0);

	drawObjects(objs);

	endFrame();
};


const void SDLRenderer::updateTrails(const vector<physobj>& objs){
	//TODO SDLRenderer::updateTrails()
};


#endif /* API_SDL */
