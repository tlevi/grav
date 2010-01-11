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
	/* get number of waiting events, or -1 for error */
	const int val = SDL_PeepEvents(NULL, 0, SDL_PEEKEVENT, SDL_EVENTMASK(SDL_KEYDOWN));
	return (val > 0);
};


/* returns next keyboard event, or NULL if none in the queue*/
const KeyEvent* const SDLInput::nextEvent(){
	static KeyEvent ev('\0', 0);

	SDL_Event SDLev;
	if (SDL_PeepEvents(&SDLev, 1, SDL_GETEVENT, SDL_EVENTMASK(SDL_KEYDOWN)) < 1) return NULL;

	const SDL_keysym& ks = SDLev.key.keysym;

	// Test the key has an ASCII value
	if ((ks.unicode & 0xFF80) != 0) return NULL;
	ev.key = ks.unicode & 0x7F;

	int mods = 0;
	if (ks.mod == 0) mods = KEYMOD_NONE;
	else{
		if (ks.mod & KMOD_CTRL) mods |= KEYMOD_CTRL;
		if (ks.mod & KMOD_ALT) mods |= KEYMOD_ALT;
		if (ks.mod & KMOD_SHIFT) mods |= KEYMOD_SHIFT;
	}
	ev.mods = mods;

	cout << ((int)ev.key) << " " << mods << endl;
	return &ev;
};


#endif /* API_SDL */
