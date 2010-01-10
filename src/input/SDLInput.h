#ifdef API_SDL
#ifndef SDLINPUT_H_
#define SDLINPUT_H_

#include "Input.h"
#include <SDL.h>


class SDLInput: public Input{
public:
	SDLInput();
	~SDLInput();
};


#endif /* SDLINPUT_H_ */
#endif /* API_SDL */
