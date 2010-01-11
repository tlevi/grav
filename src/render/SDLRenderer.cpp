#ifdef API_SDL
#include "SDLRenderer.h"


SDLRenderer::SDLRenderer(SDLInput& sdlinput){
	SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	// TODO Auto-generated constructor stub
};


SDLRenderer::~SDLRenderer(){
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
};


const void SDLRenderer::drawObjects(const vector<physobj>& objs){
};


const void SDLRenderer::updateTrails(const vector<physobj>& objs){
};


const void SDLRenderer::fadeTrails(){
};


const void SDLRenderer::clearTrails(){
};


const bool SDLRenderer::requestScreen(){
	return false;
};


const void SDLRenderer::startFrame(){
};


const void SDLRenderer::endFrame(){
};


#endif /* API_SDL */
