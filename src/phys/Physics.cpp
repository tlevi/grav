#include "Physics.h"
#include "../shared.h"
#include "../Config.h"


#define G real(6.67e-11)
#define TD real(1e2 * 4.0)


vector<physobj> Physics::objs;
vector2 Physics::boxsz(100, 100);
vector2 Physics::boxmin(0, 0);
vector2 Physics::boxmax(boxmin+boxsz);
real Physics::td(TD);
real Physics::tdsqr(TD*TD);
bool Physics::objschanged(true);
unsigned long Physics::arrcnt(0);

areal* __restrict Physics::pposx(NULL);
areal* __restrict Physics::pposy(NULL);
areal* __restrict Physics::paccx(NULL);
areal* __restrict Physics::paccy(NULL);
areal* __restrict Physics::pmass(NULL);
areal* __restrict Physics::prcpmss(NULL);
areal* __restrict Physics::pradius(NULL);
areal* __restrict Physics::pposox(NULL);
areal* __restrict Physics::pposoy(NULL);
v4sf* __restrict Physics::pvposx(NULL);
v4sf* __restrict Physics::pvposy(NULL);
v4sf* __restrict Physics::pvaccx(NULL);
v4sf* __restrict Physics::pvaccy(NULL);
v4sf* __restrict Physics::pvmass(NULL);
v4sf* __restrict Physics::pvrcpmss(NULL);
v4sf* __restrict Physics::pvradius(NULL);
v4sf* __restrict Physics::pvposox(NULL);
v4sf* __restrict Physics::pvposoy(NULL);


const void Physics::Initialise(){
	const int width = atoi(Config::get("r_width").c_str());
	const int height = atoi(Config::get("r_height").c_str());

	if (width != 0 && height != 0){
		boxsz.x = sqrt(boxsz.x * boxsz.y * width / height);
		boxsz.y = boxsz.x * height / width;

		cout << "Physics: physbox dimensions are " << boxsz.x << ", " << boxsz.y << endl;
		boxmax = boxmin + boxsz;
	}

	const int maxobjs = atoi(Config::get("objs").c_str());
	const real maxradius = 4.0;
	const real minradius = 1.0;

	for (int i=0; i < maxobjs; i++){
		Physics::addObject(minradius + FRAND * (maxradius-minradius));
	}
};


const void Physics::updatePosition(){
	const real tdsqr = Physics::tdsqr;
	const int max = objs.size();

	for (int i=0; i < max; i++){
		const real pox = pposx[i];
		pposx[i] += (pposx[i] - pposox[i]) + paccx[i] * tdsqr;
		pposox[i] = pox;
//	}

//	for (int i=0; i < max; i++){
		const real poy = pposy[i];
		pposy[i] += (pposy[i] - pposoy[i]) + paccy[i] * tdsqr;
		pposoy[i] = poy;
	}
};


const void Physics::updateAcceleration(){
	const int max = objs.size();
	const int vcnt = (max % 4 == 0) ? max/4 : (max+3) / 4;

	#pragma omp parallel for
	for (int i=0; i < vcnt; i++){
		v4sf ax = (v4sf){0, 0, 0, 0};
		v4sf ay = (v4sf){0, 0, 0, 0};

		const v4sf ix = pvposx[i];
		const v4sf iy = pvposy[i];

		for (int k=0; k < vcnt; k++){
			const v4sf dx = ix - pvposx[k];
			const v4sf dy = iy - pvposy[k];

			//BUGBUG: this hack prevents crazy & inf acceleration values
			const v4sf distsqr = MAX((v4sf){1.0, 1.0, 1.0, 1.0}, dx*dx + dy*dy);

			const v4sf gravfactor = pvmass[k] * (v4sf){G, G, G, G} / (distsqr * sqrt(distsqr));

			ax += dx * gravfactor;
			ay += dy * gravfactor;
		}

		pvaccx[i] = ax;
		pvaccy[i] = ay;
	}
};


const void Physics::addObject(const real radius){
	physobj newobj(radius);
	newobj.p = vector2(FRAND, FRAND) * (boxsz - vector2(radius*2, radius*2)) + boxmin + vector2(radius, radius);

	objs.push_back(newobj);
	reallocate();

	const int i = objs.size() - 1;
	const physobj& A = objs[i];
	pposx[i] = A.p.x;
	pposy[i] = A.p.y;
	pmass[i] = A.mass;
	prcpmss[i] = 1.0 / A.mass;
	pradius[i] = A.radius;
	pposox[i] = A.p.x;
	pposoy[i] = A.p.y;

	objschanged = true;
};


const void Physics::delObject(const physobj& oldobj){
/*	const int max = objs.size();
	int idx = 0;

	while (idx < max){
		if (&oldobj == &objs[idx]) break;
		else idx++;
	}

	delObject(idx);*/
};


const void Physics::delObject(const int idx){
/*	objs.erase(objs.begin()+idx);
	reallocate();
	objschanged = true;*/
};


const void Physics::changeTimeScale(const real factor){
	td *= factor;
	tdsqr = td*td;

	const int max = objs.size();
	for (int i=0; i < max; i++){
//		obj.po = obj.p + (obj.po - obj.p) * factor;
		pposox[i] = pposx[i] + (pposox[i] - pposx[i]) * factor;
		pposoy[i] = pposy[i] + (pposoy[i] - pposy[i]) * factor;
	}
};


const void Physics::screenCollide(){
	const int max = objs.size();

	for (int i=0; i < max; i++){
		const real minx = boxmin.x + pradius[i];
		const real maxx = boxmax.x - pradius[i];

		if (pposx[i] >= maxx){
//			po.x = p.x + max.x - po.x; p.x = max.x;
			pposox[i] = pposx[i] + maxx - pposox[i];
			pposx[i] = maxx;
		}
		if (pposx[i] < minx){
//			po.x = p.x + minx - po.x; p.x = minx;
			pposox[i] = pposx[i] + minx - pposox[i];
			pposx[i] = minx;
		}
	}

	for (int i=0; i < max; i++){
		const areal miny = boxmin.y + pradius[i];
		const areal maxy = boxmax.y - pradius[i];

		if (pposy[i] >= maxy){
//			po.x = p.y + max.y - po.y; p.y = max.y;
			pposoy[i] = pposy[i] + maxy - pposoy[i];
			pposy[i] = maxy;
		}
		if (pposy[i] < miny){
//			po.y = p.y + miny - po.y; p.y = miny;
			pposoy[i] = pposy[i] + miny - pposoy[i];
			pposy[i] = miny;
		}
	}
};


const void Physics::advanceTicks(int ticks){
	unpack();
	while (ticks > 0){
		updateAcceleration();
		updatePosition();
		screenCollide();
		ticks--;
	}
	pack();
};


const void Physics::Shutdown(){
	if (pposx != NULL) FREE(pposx);
	if (pposy != NULL) FREE(pposy);
	if (paccx != NULL) FREE(paccx);
	if (paccy != NULL) FREE(paccy);
	if (pmass != NULL) FREE(pmass);
	if (prcpmss != NULL) FREE(prcpmss);
	if (pradius != NULL) FREE(pradius);
	if (pposox != NULL) FREE(pposox);
	if (pposoy != NULL) FREE(pposoy);
	copyptrs();
};


const void Physics::reallocate(){
	if (arrcnt >= objs.capacity()) return;

	const unsigned long count = objs.capacity();
	const unsigned long sz = (count > 16/sizeof(real)) ? count*sizeof(real) : 16;

	pposx = static_cast<areal*>(arealloc(pposx, sz));
	pposy = static_cast<areal*>(arealloc(pposy, sz));
	paccx = static_cast<areal*>(arealloc(paccx, sz));
	paccy = static_cast<areal*>(arealloc(paccy, sz));
	pmass = static_cast<areal*>(arealloc(pmass, sz));
	prcpmss = static_cast<areal*>(arealloc(prcpmss, sz));
	pradius = static_cast<areal*>(arealloc(pradius, sz));
	pposox = static_cast<areal*>(arealloc(pposox, sz));
	pposoy = static_cast<areal*>(arealloc(pposoy, sz));
	copyptrs();

	arrcnt = objs.capacity();
};


const void Physics::copyptrs(){
	pvposx = reinterpret_cast<v4sf*>(pposx);
	pvposy = reinterpret_cast<v4sf*>(pposy);
	pvaccx = reinterpret_cast<v4sf*>(paccx);
	pvaccy = reinterpret_cast<v4sf*>(paccy);
	pvmass = reinterpret_cast<v4sf*>(pmass);
	pvrcpmss = reinterpret_cast<v4sf*>(prcpmss);
	pvradius = reinterpret_cast<v4sf*>(pradius);
	pvposox = reinterpret_cast<v4sf*>(pposox);
	pvposoy = reinterpret_cast<v4sf*>(pposoy);
};


const void Physics::unpack(){
};


const void Physics::pack(){
	const int max = objs.size();
	for (int i=0; i < max; i++){
		objs[i].p.x = pposx[i];
		objs[i].p.y = pposy[i];
	}
};
