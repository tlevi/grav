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
/*	for (int i=0; i < max; i++){
		const vector2 po = o.p;
		o.p += (o.p - o.po) + o.a * tdsqr;
		o.po = po;
	}*/
	/* broken into two loops so auto-vectoriser digests */
	for (int i=0; i < max; i++){
		const real pox = pposx[i];
		pposx[i] += (pposx[i] - pposox[i]) + paccx[i] * tdsqr;
		pposox[i] = pox;
	}
	for (int i=0; i < max; i++){
		const real poy = pposy[i];
		pposy[i] += (pposy[i] - pposoy[i]) + paccy[i] * tdsqr;
		pposoy[i] = poy;
	}
};


const void Physics::updateAcceleration(){
	int max = objs.size();
	if (max % 4 != 0) max = ((max + 3)/4)*4;
	if (max == 0) max = 4;

	for (int i=0; i < max; i++){
		real ax = 0;
		real ay = 0;

		const real tmp = pmass[i];
		pmass[i] = 0;

		for (int k=0; k < max; k++){
			const real dx = pposx[k] - pposx[i];
			const real dy = pposy[k] - pposy[i];

			const real GxBmass = pmass[k] * G;

			const real distsqr = dx*dx + dy*dy;
			if (distsqr <= 1.0) continue; //BUGBUG, just fixes crazy accel for now

			const real gravfactor =  GxBmass / (distsqr * sqrt(distsqr));

			ax += dx * gravfactor;
			ay += dy * gravfactor;
		}

		pmass[i] = tmp;

		paccx[i] = ax;
		paccy[i] = ay;
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

	areal* __restrict const pposox = Physics::pposox;
	const areal* __restrict const pposx = Physics::pposx;
	areal* __restrict const pposoy = Physics::pposoy;
	const areal* __restrict const pposy = Physics::pposy;

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
		const real miny = boxmin.y + pradius[i];
		const real maxx = boxmax.x - pradius[i];
		const real maxy = boxmax.y - pradius[i];

		if (pposx[i] >= maxx){
//			po.x = p.x + max.x - po.x; p.x = max.x;
			pposox[i] = pposx[i] + maxx - pposox[i];
			pposx[i] = maxx;
		}
		else if (pposx[i] < minx){
//			po.x = p.x + minx - po.x; p.x = minx;
			pposox[i] = pposx[i] + minx - pposox[i];
			pposx[i] = minx;
		}

		if (pposy[i] >= maxy){
//			po.y = p.y + maxy - po.y; p.y = maxy;
			pposoy[i] = pposy[i] + maxy - pposoy[i];
			pposy[i] = maxy;
		}
		else if (pposy[i] < miny){
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
	if (pposx != NULL) free(pposx);
	if (pposy != NULL) free(pposy);
	if (paccx != NULL) free(paccx);
	if (paccy != NULL) free(paccy);
	if (pmass != NULL) free(pmass);
	if (prcpmss != NULL) free(prcpmss);
	if (pradius != NULL) free(pradius);
	if (pposox != NULL) free(pposox);
	if (pposoy != NULL) free(pposoy);
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

	arrcnt = objs.capacity();
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
