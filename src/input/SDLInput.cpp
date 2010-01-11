#ifdef API_SDL
#include "SDLInput.h"
#include "../shared.h"


SDLInput::SDLInput(){
	SDL_EnableUNICODE(1);
	if (SDL_EnableUNICODE(-1) != 1) fatalError("Could not enable Unicode.\n");
};


SDLInput::~SDLInput(){
	// TODO Auto-generated destructor stub
};


const void SDLInput::Reinitialise(){
	SDL_PumpEvents();
};


const void SDLInput::PumpEvents(){
	SDL_PumpEvents();
};


/* call PumpEvents before calling this */
const bool SDLInput::hasNext() const{
	return bool(SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_KEYDOWN)));
};


/* returns next keyboard event, or NULL if none in the queue*/
const KeyEvent* const SDLInput::nextEvent(){
	static SDL_Event SDLev;
	if(SDL_PeepEvents(&SDLev, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_KEYDOWN)) < 1) return NULL;

	int mods = KEYMOD_NONE;
	if(SDLev.key.keysym.mod|KMOD_CTRL) mods |= KEYMOD_CTRL;
	if(SDLev.key.keysym.mod|KMOD_ALT) mods |= KEYMOD_ALT;
	if(SDLev.key.keysym.mod|KMOD_SHIFT) mods |= KEYMOD_SHIFT;

	char key = SDLev.key.keysym.unicode & 0x7F;

	static KeyEvent ev(key, mods);
	return &ev;
};


#endif /* API_SDL */
