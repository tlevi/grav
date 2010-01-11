#include "shared.h"
#include "render/Renderer.h"
#include "input/Input.h"
#include <unistd.h>
//#include <locale.h>


#ifdef API_GLUT
#include <GL/freeglut.h>
#endif
#ifdef API_SDL
#include <SDL.h>
#endif


Input* pInput(NULL);
Renderer* pRenderer(NULL);


static void loopWork();
static void cleanexit();
static const void mainLoop();


static const bool quitKey(const KeyEvent& kev){
//	if (kev.key == 'q' && (kev.mods&KEYMOD_CTRL)) return true;
//	if (kev.key == 'Q' && (kev.mods&KEYMOD_CTRL)) return true;
	if (kev.key == 0x11) return true;
	return false;
};


static void loopWork(){
	pInput->PumpEvents();

	while (pInput->hasNext()){
		const KeyEvent* const pEv = pInput->nextEvent();
		if (pEv == NULL) continue;
		if (quitKey(*pEv)) exit(EXIT_SUCCESS);
	}

	pRenderer->startFrame();
	pRenderer->endFrame();
};


static void cleanexit(){
	delete pRenderer;
	delete pInput;

	#ifdef SDL_API
	SDL_Quit();
	#endif
};


static const void mainLoop(){
	while (1){
		loopWork();
		usleep(10000);
	}
};


int main(int argc, char** argv){
	atexit(cleanexit);

	#ifdef API_GLUT
	glutInit(&argc, argv);
	#endif
	#ifdef API_SDL
	SDL_Init(SDL_INIT_VIDEO);
	#endif

	pInput = Input::getInput();
	pRenderer = Renderer::getRenderer(*pInput);

	#ifdef API_SDL
	mainLoop();
	#endif
	#ifdef API_GLUT
	if (0) mainLoop();
	glutIdleFunc(loopWork);
	glutMainLoop();
	#endif

	return EXIT_SUCCESS;
};
