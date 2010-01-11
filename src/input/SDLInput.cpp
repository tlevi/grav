#ifdef API_SDL
#include "SDLInput.h"

/* TODO: should check UNICOE is enabled and output an error if false*/
SDLInput::SDLInput(){
	SDL_EnableUNICODE(1);
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
const KeyEvent& SDLInput::nextEvent(){
	static SDL_Event SDLev;

	if(SDL_PeepEvents(&SDLev, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_KEYDOWN)) < 1) {
		static KeyEvent errorEvent('\0', 0);
		return errorEvent;
	}

	int mods = KEYMOD_NONE;
	if(SDLev.key.keysym.mod|KMOD_CTRL) mods |= KEYMOD_CTRL;
	if(SDLev.key.keysym.mod|KMOD_ALT) mods |= KEYMOD_ALT;
	if(SDLev.key.keysym.mod|KMOD_SHIFT) mods |= KEYMOD_SHIFT;

	char key = SDLev.key.keysym.unicode & 0x7F;

	static KeyEvent ev(key, mods);
	return ev;
};


#endif /* API_SDL */
