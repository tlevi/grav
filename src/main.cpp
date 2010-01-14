#include "shared.h"
#include "render/Renderer.h"
#include "input/Input.h"
#include <unistd.h>
#include "phys/Physics.h"
#include <ctime>
#include "Config.h"


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
	usleep(1);
};


static void cleanexit(){
	delete pRenderer;
	delete pInput;

	#ifdef API_SDL
	SDL_Quit();
	#endif
	#ifdef API_GLUT
	glutIdleFunc(NULL);
	#endif
};


static const void mainLoop(){
	ticks = getticks();
	while (1){
		loopWork();
	}
};


int main(int argc, char** argv){
	atexit(cleanexit);

	int max = atoi(Config::get("objs")->c_str());
	if (max == 0){
		Config::put("objs", "20");
		max = 20;
	}

	const float maxradius = 5.0;
	const float minradius = 1.0;
	for (int i=0; i < max; i++){
		const float r = minradius + (rand()/RAND_MAX) * (maxradius-minradius);
		Physics::addObject(r);
	}

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
	ticks = getticks();
	glutIdleFunc(loopWork);
	glutMainLoop();
	#endif

	return EXIT_FAILURE;
};
