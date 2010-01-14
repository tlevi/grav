#ifdef API_SDL
#include "SDLInput.h"
#include "../shared.h"


SDLInput::SDLInput(){
	if (!SDL_WasInit(SDL_INIT_VIDEO))
		fatalError("SDLInput: Video & events system has not been initialised.\n");

	SDL_EnableUNICODE(1);

	if (SDL_EnableUNICODE(-1) != 1)
		fatalError("SDLInput: Could not enable Unicode.\n");
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
	/* get number of waiting events, or -1 for error */
	const int val = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_KEYDOWN));
	return (val > 0);
};


/* returns next keyboard event, or NULL if none in the queue*/
const bool SDLInput::nextEvent(KeyEvent& kev){
	SDL_Event SDLev;
	if (SDL_PeepEvents(&SDLev, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_KEYDOWN)) < 1) return NULL;

	const SDL_keysym& ks = SDLev.key.keysym;

	// Test the key has an ASCII value
	if ((ks.unicode & 0xFF80) != 0) return false;
	kev.key = ks.unicode & 0x7F;

	int mods = 0;
	if (ks.mod == 0) mods = KEYMOD_NONE;
	else{
		if (ks.mod & KMOD_CTRL) mods |= KEYMOD_CTRL;
		if (ks.mod & KMOD_ALT) mods |= KEYMOD_ALT;
		if (ks.mod & KMOD_SHIFT) mods |= KEYMOD_SHIFT;
	}
	kev.mods = mods;

	return true;
};


#endif /* API_SDL */
