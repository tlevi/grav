#include "shared.h"
#include "render/Renderer.h"
#include "input/Input.h"
#include "unistd.h"
#include <GL/freeglut.h>


Input* pInput(NULL);
Renderer* pRenderer(NULL);


static void loopWork(){
	pInput->PumpEvents();

	while (pInput->hasNext()){
		const KeyEvent* const pEv = pInput->nextEvent();
		if (pEv == NULL) exit(0);
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
		usleep(1);
	}
};


int main(int argc, char** argv){
	atexit(cleanexit);

	#ifdef API_GLUT
	glutInit(&argc, argv);
	#endif
	#ifdef API_SDL
	SDL_Init(SDL_INIT_TIMER);
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

	return 0;
};
