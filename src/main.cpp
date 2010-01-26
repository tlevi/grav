#include "shared.h"
#include "render/Renderer.h"
#include "input/Input.h"
#include <unistd.h>
#include "phys/Physics.h"
#include <ctime>
#include "Config.h"
#include "vector2.h"

#ifdef API_GLUT
#include <GL/freeglut.h>
#endif
#ifdef API_SDL
#include <SDL.h>
#endif


static Input* pInput(NULL);
static Renderer* pRenderer(NULL);
static unsigned long ticks = 0;
static unsigned long framecount;


static void loopWork();
static void cleanexit();
static const void mainLoop();


static const bool quitKey(const KeyEvent& kev){
	if (kev.key == 0x11) return true;
	return false;
};


__attribute__ ((unused))
static void sdlfps(const unsigned long now){
	#ifdef API_SDL
	static bool firstrun(true);
	static bool sdl_drawfps(false);
	if (firstrun){
		firstrun = false;
		sdl_drawfps = (Config::get("sdl_fps") == "true");
		fpstick = getticks();
		framecount = 0;
	}
	if (!sdl_drawfps) return;

	static unsigned long fpstick;
	if (now - fpstick < 2000) return;
	const double secs = int(double(now - fpstick) / 10.0) / 100.0;
	const double fps = int(double(framecount) * 10.0 / secs) / 10.0;
	cout << "grav: " << framecount << " frames in " << secs << " seconds = " << fps << " FPS\n";
	fpstick = now;
	framecount = 0;
	#endif
};


static void loopWork(){
	pInput->PumpEvents();

	while (pInput->hasNext()){
		KeyEvent kev;
		if (!pInput->nextEvent(kev)) continue;
		if (quitKey(kev)){
			#ifdef API_GLUT
			glutLeaveMainLoop();
			#endif
			exit(EXIT_SUCCESS);
		}
	}

	const vector<physobj>& vec = Physics::getObjs();
	const unsigned long newticks = getticks();
	sdlfps(newticks);

	Physics::advanceTicks(newticks - ticks);
//	cout << "phys time: " << (getticks()-newticks) << endl;
	ticks = newticks;

	pRenderer->doDrawing(vec);
	framecount++;
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

	Physics::Shutdown();
};


static const void mainLoop(){
	ticks = getticks();
	while (1) loopWork();
};


int main(int argc, char** argv){
	atexit(cleanexit);
	srand(time(NULL));

	Physics::Initialise();

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
