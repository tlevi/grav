#include "shared.h"
#include "render/Renderer.h"
#include "input/Input.h"
#include <unistd.h>
#include "phys/Physics.h"
#include <ctime>


#ifdef API_GLUT
#include <GL/freeglut.h>
#endif
#ifdef API_SDL
#include <SDL.h>
#endif


Input* pInput(NULL);
Renderer* pRenderer(NULL);
unsigned long ticks = 0;


static void loopWork();
static void cleanexit();
static const void mainLoop();


static const bool quitKey(const KeyEvent& kev){
	if (kev.key == 0x11) return true;
	return false;
};


static void loopWork(){
	pInput->PumpEvents();

	while (pInput->hasNext()){
		KeyEvent kev;
		if (!pInput->nextEvent(kev)) continue;
		if (quitKey(kev)) exit(EXIT_SUCCESS);
	}

	const vector<physobj>& vec = Physics::getObjs();
	const unsigned int newticks = getticks();

	while (ticks < newticks){
		Physics::advanceTick();
		pRenderer->updateTrails(vec);
		ticks++;
	}

	pRenderer->doDrawing(vec);
};


static void cleanexit(){
	delete pRenderer;
	delete pInput;

	#ifdef API_SDL
	SDL_Quit();
	#endif
};


static const void mainLoop(){
	ticks = getticks();
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
	SDL_Init(SDL_INIT_VIDEO);
	#endif

	pInput = Input::getInput();
	pRenderer = Renderer::getRenderer(*pInput);

	pRenderer->requestScreen();

	#ifdef API_SDL
	mainLoop();
	#endif
	#ifdef API_GLUT
	if (0) mainLoop();
	glutIdleFunc(loopWork);
	glutMainLoop();
	#endif

	return EXIT_FAILURE;
};
