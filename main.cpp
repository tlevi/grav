#include "main.h"
#include "render.h"
#include "object.h"
#include "time.h"
#include "physic.h"
#include "unistd.h"


void keyCheck(void);
void runLoop(void);
bool initScreen(void);
inline void updateTime(void);
void initTime(void);
void restart(void);
void createCollideFix(object * pThis);

void Global::Init(void){
	return;
};


bool Global::bIsRunning = true;
bool Global::bPaused = false;
bool Global::bDirty = false;
short Global::screenWidth = 1620;
short Global::screenHeight = 950;
int Global::objectcount = 0;
int Global::maxobjects = 16;
char Global::ubSpace = 0;
uInt Global::framesn = 0;
char Global::kbSpace = 0;
char Global::ubEnter = 0;
char Global::kbEnter = 0;
char Global::kbQ = 0;
char Global::ubQ = 0;
char Global::ubD = 0;
char Global::kbD = 0;
char Global::kbM = 0;
char Global::ubM = 0;
char Global::kbPlus = 0;
char Global::ubPlus = 0;
char Global::kbMinus =0;
char Global::ubMinus =0;
bool Global::bUpdateBuffer = true;
bool Global::bRandomMass = false;
uInt Global::timenow = 0;
uInt Global::timeold = 0;
uInt Global::tick = 0;
uInt Global::ticksp = 0;
uChar * Global::keyboard = NULL;
SDL_Surface * Global::screen = NULL;
SDL_Surface * Global::buffer = NULL;
uInt * Global::pBuff = NULL;
uInt Global::timedelta = 0;
uInt Global::tickfade = 0;
float Global::timedeltasec = 0;
double Global::frametime = 0;


int main(int argc, char *argv[]){
	Global::Init();
	inittbl();
	atexit(SDL_Quit);
	initTime();

	srand((uLong)time(NULL));

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		fprintf(stdout, "Unable to init SDL: %s\n", SDL_GetError());
		exit(1);
	}

	if (initScreen() == false){
		fprintf(stdout, "Can't get screen. (exit)\n");
		exit(1);
	}

	SDL_ShowCursor(0);
	object::makeObjects();
	SDL_Delay(10);
	runLoop();

	exit(0);
	return 0;
}


void restart(void){
	SDL_FillRect(Global::buffer, NULL, 0);
	while (Global::objectcount > 0){
		delete pObjects[Global::objectcount-1];
	}
	srand((uLong)time(NULL));
	SDL_Delay(10);
	updateTime();
}


void runLoop(void){
	Global::tickfade = SDL_GetTicks();

	while (Global::bIsRunning == true){
		if (SDL_GetTicks() - Global::timenow == 0) usleep(1);
		updateTime();
		SDL_PumpEvents();
		keyCheck();

		while (Global::bPaused == true){			SDL_PumpEvents();
			keyCheck();
			if (Global::bIsRunning == false) return;
			if (Global::bPaused == false){
				Global::ticksp += SDL_GetTicks() - Global::tick;
				Global::timenow = SDL_GetTicks();
			}
			else SDL_Delay(10);
		}

		physics::updatePhysics(Global::tick - Global::ticksp);

		//Clear out sections of screen before we don't know where we drew ;-)
		if (Global::bUpdateBuffer == false){
			for (short i=0; i < Global::objectcount; i++){
				SDL_Rect tmp;
				const short s = short(pObjects[i]->physic->radius * 2);
				tmp.x = MIN(pObjects[i]->display.ox, pObjects[i]->display.x) - s;
				tmp.y = MIN(pObjects[i]->display.oy, pObjects[i]->display.y) - s;
				tmp.w = abs(pObjects[i]->display.ox - pObjects[i]->display.x) + s*2;
				tmp.h = abs(pObjects[i]->display.oy - pObjects[i]->display.y) + s*2;
				SDL_FillRect(Global::screen, &tmp, 0);
			}
		}

		object::updateObjects();
		drawObjects();
	}
}


bool initTrailSurface(void){
	SDL_PixelFormat * sf = Global::screen->format;
	Global::buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, Global::screenWidth, Global::screenHeight, 32, sf->Rmask, sf->Gmask, sf->Bmask, sf->Amask);
	if (Global::buffer == NULL) return false;
	else{
		SDL_FillRect(Global::buffer, NULL, 0);
		Global::pBuff = (uInt*) Global::buffer->pixels;
		return true;
	}
}


bool initScreen(void){
	Global::screen = SDL_SetVideoMode(Global::screenWidth, Global::screenHeight, 32, SDL_SWSURFACE);
	if (Global::screen == NULL) return false;
	else{
		if (initTrailSurface() == true) return true;
		else return false;
	}
}


void keyCheck(void){
	if (!SDL_PollEvent(NULL)) return;

	Global::keyboard = SDL_GetKeyState(NULL);
	if (Global::keyboard[SDLK_ESCAPE]){
		Global::bIsRunning = false;
	}
	else if (Global::keyboard[SDLK_SPACE] != Global::ubSpace){
		Global::ubSpace = (Global::ubSpace==1)? 0 : 1;
		Global::kbSpace++;
		if (Global::kbSpace == 2){
			Global::kbSpace = 0;
			Global::bPaused = !(Global::bPaused);
		}
	}
	else if ((Global::keyboard[SDLK_RETURN]|Global::keyboard[SDLK_KP_ENTER]) != Global::ubEnter){
		Global::ubEnter = (Global::ubEnter==1)? 0 : 1;
		Global::kbEnter++;
		if (Global::kbEnter == 2){
			Global::kbEnter = 0;
			object * pThis = new object();
			createCollideFix(pThis);
			if (SDL_GetModState() & KMOD_RALT){
				pThis->physic->np.x += physics::td * (FRAND - 0.5f) / 500;
				pThis->physic->np.y += physics::td * (FRAND - 0.5f) / 500;
			}
		}
	}

	else if (Global::keyboard[SDLK_w]){
		SDL_FillRect(Global::buffer, NULL, 0);
		Global::bDirty = true;
	}
	else if (Global::keyboard[SDLK_r]){
		restart();
		Global::bDirty = true;
	}
	else if (Global::keyboard[SDLK_q] != Global::ubQ){
		Global::ubQ = (Global::ubQ==1)? 0 : 1;
		Global::kbQ++;
		if (Global::kbQ == 2){
			Global::kbQ = 0;
			Global::bUpdateBuffer = !(Global::bUpdateBuffer);
			Global::bDirty = true;
		}
	}
	else if (Global::keyboard[SDLK_d] != Global::ubD){
		Global::ubD = (Global::ubD==1)? 0 : 1;
		Global::kbD++;
		if (Global::kbD == 2){
			Global::kbD = 0;
			if (Global::objectcount == 0) return;
			float r = rand() / float(RAND_MAX) * Global::objectcount;
			uShort i = (uShort) r;
			delete pObjects[i];
		}
	}
	else if (Global::keyboard[SDLK_m] != Global::ubM){
		Global::ubM = (Global::ubM==1)? 0 : 1;
		Global::kbM++;
		if (Global::kbM == 2){
			Global::kbM = 0;
			Global::bRandomMass = !(Global::bRandomMass);
		}
	}
	else if (Global::keyboard[SDLK_KP_PLUS] != Global::ubPlus){
		Global::ubPlus = (Global::ubPlus==1)? 0 : 1;
		Global::kbPlus++;
		if (Global::kbPlus == 2){
			Global::kbPlus = 0;
		}
		physics::physTimeMultiplier *= 2.0f;
	}
	else if (Global::keyboard[SDLK_KP_MINUS] != Global::ubMinus){
		Global::ubMinus = (Global::ubMinus==1)? 0 : 1;
		Global::kbMinus++;
		if (Global::kbMinus == 2){
			Global::kbMinus = 0;
		}
		physics::physTimeMultiplier *= 0.5f;
	}
}


void createConstraint(physics * pA, physics * pB){
	vector2 fix = pB->np - pA->np;
	const float fixc = 1.0f - (pB->radius - pA->radius) / LENGTH(fix);
	pA->np += fix * fixc;
	return;
}


void createCollideFix(object * pThis){
	pThis->physic->np.x = (FRAND - 0.5f) * (float)Global::screenWidth;
	pThis->physic->np.y = (FRAND - 0.5f) * (float)Global::screenHeight;
/*	bool bCollide = true;
	while (bCollide == true){
		bCollide = false;
		for (unsigned short i=0; i<Global::objectcount; i++){
			if (pObjects[i] == pThis) continue;
			const vector2 dist = pThis->physic->np - pObjects[i]->physic->np;
			const float distSqr = SQRLENGTH(dist);
			const float t = pThis->physic->radius + pObjects[i]->physic->radius;
			const float minDist = t*t;
			if (minDist > distSqr){
				createConstraint(pThis->physic, pObjects[i]->physic);
				bCollide = true;
			}
		}
		if (SDL_GetTicks() - Global::timenow > 20) break;
	}*/
	pThis->physic->clearVelocity();
}


void updateTime(void){
	Global::tick = SDL_GetTicks();
	Global::timeold = Global::timenow;
	Global::timenow = SDL_GetTicks();
	Global::timedelta = Global::timenow - Global::timeold;
	Global::timedeltasec = (float)Global::timedelta / 1000.0f;
	Global::framesn++;
	Global::frametime += (double) Global::timedelta;
}


void initTime(void){
	Global::timeold = 0;
	Global::timenow = 0;
	Global::timedelta = 0;
	Global::timedeltasec = 0;
	Global::frametime = 0;
}
