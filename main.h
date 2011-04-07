#ifndef _MAIN_H_
#define _MAIN_H_

#include "shared.h"

class Global{
	public:
		static void Init(void);

		static double frametime;
		static SDL_Surface * screen;
		static SDL_Surface * buffer;
		static uInt * pBuff;
		static uChar * keyboard;
		static uInt timenow;
		static uInt timeold;
		static uInt timedelta;
		static uInt tick;
		static uInt ticksp;
		static uInt tickfade;
		static uInt framesn;
		static float timedeltasec;
		static short screenWidth;
		static short screenHeight;
		static int maxobjects;
		static int objectcount;
		static int physDetail;
		static bool bIsRunning;
		static bool bPaused;
		static bool bUpdateBuffer;
		static bool bDirty;
		static bool bRandomMass;
		static bool frameskip;
		static char ubSpace;
		static char kbSpace;
		static char ubEnter;
		static char kbEnter;
		static char kbQ;
		static char ubQ;
		static char kbD;
		static char ubD;
		static char kbM;
		static char ubM;
		static char kbPlus;
		static char ubPlus;
		static char kbMinus;
		static char ubMinus;
};

#endif //_MAIN_H_
