#include "render.h"
#include "main.h"
#include "object.h"
#include "physic.h"
#include <cmath>


uChar addtbl[512];
uChar drktbl[256];


const inline uInt additive(const uInt add, const uInt bg){
	const uChar r = addtbl[(bg>>16) + (add>>16)];
	const uChar g = addtbl[((bg>>8)&0xff) + ((add>>8)&0xff)];
	const uChar b = addtbl[(bg&0xff) + (add&0xff)];
	return (((r << 8) | g) << 8) | b;
};


const void inittbl(void){
	for (short i=0; i < 256; i++){
		addtbl[i] = uChar(i);
		addtbl[256+i] = uChar(i);
		if (i > 1) drktbl[i] = uChar(i - 1);
		else drktbl[i] = 0;
	}
};


const inline uInt getBuffPixel(const short x, const short y){
	return Global::pBuff[y * Global::screenWidth + x];
};


const inline void setBuffPixel(const short x, const short y, const uInt px){
	Global::pBuff[y * Global::screenWidth + x] = px;
};


const void drawCircle_Surface(object * pThis){
	const double radius = pThis->physic->radius;
	uInt color = pThis->display.color;

	SDL_Rect thisrow;
	thisrow.h = 1;
	if (!color)	color = SDL_MapRGB(pThis->pSurf->format, 0, 0, 255); //Blue
	float x = 0;

	short test = short(radius);
	for (short i=short(-radius); i<=test; i++){
		thisrow.y = radius + i;
		if (thisrow.y < 0) continue;
		x = sqrtf(float(radius*radius - i*i));
		thisrow.x = short(radius - x);
		thisrow.w = short(x*2 - 0.5);
		SDL_FillRect(pThis->pSurf, &thisrow, color);
	}

	return;
}


const void darkenTrails(void){
	const int its = ((Global::tick - Global::tickfade) / 50);

	if (its > 1){
#pragma omp parallel for
		for (short y=0; y < Global::screenHeight; ++y){
			for (short x=0; x < Global::screenWidth; ++x){
				for (short k=0; k < its; ++k){
					const uInt src = getBuffPixel(x, y);

					const uChar r = drktbl[(src>>16) & 0xff];
					const uChar g = drktbl[(src>>8 ) & 0xff];
					const uChar b = drktbl[src       & 0xff];

					setBuffPixel(x, y, (((r<<8)|g)<<8)|b);
				}
			}
		}
		return;
	}

#pragma omp parallel for
	for (short y=0; y < Global::screenHeight; ++y){
		for (short x=0; x < Global::screenWidth; ++x){
			const uInt src = getBuffPixel(x, y);

			const uChar r = drktbl[(src>>16) & 0xff];
			const uChar g = drktbl[(src>>8 ) & 0xff];
			const uChar b = drktbl[src       & 0xff];

			setBuffPixel(x, y, (((r<<8)|g)<<8)|b);
		}
	}
};


const void updateTrails(void){
	if (Global::bUpdateBuffer == false) return;
	
	const float scrWh = float(Global::screenWidth) * 0.5;
	const float scrHh = float(Global::screenHeight) * 0.5;

	for (short i=0; i < Global::objectcount; i++){
		const object * pObj = pObjects[i];
		const physics * pPhys = pObj->physic;

		const float px = pPhys->np.x + scrWh;
		if (px < 0 || px >= Global::screenWidth) continue;

		const float py = scrHh - pPhys->np.y;
		if (py < 0 || py >= Global::screenHeight) continue;

		setBuffPixel(short(px), short(py), pObj->display.tcolor);
	}
	return;
};


const void drawObjects(void){
	if (Global::bDirty == true){
		SDL_FillRect(Global::screen, NULL, 0);
	}

	//Blit the trails across again
	if (Global::tick - Global::tickfade >= 50){
		darkenTrails();
		if (Global::bUpdateBuffer == true){
			SDL_BlitSurface(Global::buffer, NULL, Global::screen, NULL);
		}
	}
	else if (Global::bUpdateBuffer == true && Global::bDirty == true){
		SDL_BlitSurface(Global::buffer, NULL, Global::screen, NULL);
	}
	else if (Global::bUpdateBuffer == true){
		for (short i=0; i < Global::objectcount; i++){
			const short s = short(pObjects[i]->physic->radius * 2);

			SDL_Rect tmp;
			tmp.x = MIN(pObjects[i]->display.ox, pObjects[i]->display.x) - s;
			tmp.y = MIN(pObjects[i]->display.oy, pObjects[i]->display.y) - s;
			tmp.w = abs(pObjects[i]->display.ox - pObjects[i]->display.x) + s*2;
			tmp.h = abs(pObjects[i]->display.oy - pObjects[i]->display.y) + s*2;
			SDL_BlitSurface(Global::buffer, &tmp, Global::screen, &tmp);
		}
	}

	//Blit objects onto screen
	#pragma omp parallel for
	for (short i=0; i < Global::objectcount; i++){
		SDL_Rect dst;
		const object * pObj = pObjects[i];
		const physics * pPhys = pObj->physic;
		dst.x = (short) (float(pObj->display.x) - pPhys->radius);
		dst.y = (short) (float(pObj->display.y) - pPhys->radius);
		SDL_BlitSurface(pObj->pSurf, NULL, Global::screen, &dst);
	}

	if (Global::bDirty == true){
		SDL_UpdateRect(Global::screen, 0, 0, 0, 0);
		Global::bDirty = false;
		return;
	}
	
	if (Global::tick - Global::tickfade >= 50){
		SDL_UpdateRect(Global::screen, 0, 0, 0, 0);
		Global::tickfade = Global::tick;
	}

	SDL_Rect * scrrects = new SDL_Rect[Global::objectcount];
	for (short i=0; i < Global::objectcount; i++){
		const object * pObj = pObjects[i];
		const physics * pPhys = pObj->physic;

		const short w = short(pPhys->radius + 0.5);

		scrrects[i].x = MAX(MIN(pObj->display.x, pObj->display.ox) - w, 0);
		const short maxx = MIN(MAX(pObj->display.x, pObj->display.ox) + w, Global::screenWidth);
		scrrects[i].w = maxx - scrrects[i].x;

		scrrects[i].y = MAX(MIN(pObj->display.y, pObj->display.oy) - w, 0);
		const short maxy = MIN(MAX(pObj->display.y, pObj->display.oy) + w, Global::screenHeight);
		scrrects[i].h = maxy - scrrects[i].y;
	}

	SDL_UpdateRects(Global::screen, Global::objectcount, scrrects);
	delete[] scrrects;
	scrrects = NULL;

	return;
}
