#include "Input.h"
#include "SDLInput.h"
#include "GLUTInput.h"


Input* const Input::getInput(){
#ifdef API_SDL
	return new SDLInput();
#endif
#ifdef API_GLUT
	return new GLUTInput();
#endif
};
