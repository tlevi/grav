#ifdef API_SDL
#include "SDLInput.h"


SDLInput::SDLInput(){
	// TODO Auto-generated constructor stub
};


SDLInput::~SDLInput(){
	// TODO Auto-generated destructor stub
};


const void SDLInput::Reinitialise(){
};


const void SDLInput::PumpEvents(){
};


const bool SDLInput::hasNext() const{
	return false;
};


const KeyEvent& SDLInput::nextEvent(){
	static KeyEvent ev('\0', 0);
	//TODO
	return ev;
};


#endif /* API_SDL */
