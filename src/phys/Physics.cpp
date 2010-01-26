#include "Physics.h"
#include "../shared.h"
#include "../Config.h"
#include "CDetect.h"


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
CDetect& Physics::cdetect(*(new CDetect()));

unsigned int* __restrict Physics::pids(NULL);
areal* __restrict Physics::pposx(NULL);
areal* __restrict Physics::pposxe(NULL);
areal* __restrict Physics::pposy(NULL);
areal* __restrict Physics::paccx(NULL);
areal* __restrict Physics::paccy(NULL);
areal* __restrict Physics::pmass(NULL);
areal* __restrict Physics::prcpmss(NULL);
areal* __restrict Physics::pradius(NULL);
areal* __restrict Physics::pposox(NULL);
areal* __restrict Physics::pposoy(NULL);
v4sf* __restrict Physics::pvposx(NULL);
v4sf* __restrict Physics::pvposxe(NULL);
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

//	objs.reserve(maxobjs);
//	reallocate();

	const real maxradius = 6.0;
	const real minradius = 1.0;

	for (int i=0; i < maxobjs; i++){
		Physics::addObject(minradius + FRAND * (maxradius-minradius));
	}
};

#include <cstdio>

const void Physics::updatePosition(){
	const real tdsqr = Physics::tdsqr;
	const int max = objs.size();

	const int vcnt = (max%4 == 0) ? max/4 : (max+3)/4;
	const v4sf tdsqrv = (v4sf){tdsqr, tdsqr, tdsqr, tdsqr};

//	printf("%.16e -> ", paccx[0]);
	for (int i=0; i < vcnt; i++){
		const v4sf poxv = pvposx[i];
		const v4sf poyv = pvposy[i];

//		const v4sf npx = pvposx[i] + pvposx[i] - pvposox[i];
//		const v4sf dx = pvaccx[i] * tdsqrv;

//		pvposxe[i] += ((npx + dx) - npx);
//		pvposx[i] = npx + dx;

		pvposx[i] += (pvposx[i] - pvposox[i]) + pvaccx[i] * tdsqrv;
		pvposy[i] += (pvposy[i] - pvposoy[i]) + pvaccy[i] * tdsqrv;
		pvposox[i] = poxv;
		pvposoy[i] = poyv;
	}
//	printf("%.16e\n", paccx[0]);
};


const void Physics::updateAcceleration(){
	const int max = objs.size();
	const int vcnt = (max%4 == 0) ? max/4 : (max+3)/4;

	#pragma omp parallel for
	for (int i=0; i < vcnt; i++){
		v4sf ax = (v4sf){0, 0, 0, 0};
		v4sf ay = (v4sf){0, 0, 0, 0};

		const v4sf ix = pvposx[i];
		const v4sf iy = pvposy[i];

		for (int k=0; k < vcnt; k++){
			const v4sf dx = pvposx[k] - ix;
			const v4sf dy = pvposy[k] - iy;

			//objects don't accelerate each other if closer than radii
			const v4sf dmin = pvradius[i] + pvradius[k];

			const v4sf distsqr = VMAX(dmin, dx*dx + dy*dy);

			const v4sf gravfactor = pvmass[k] * (v4sf){G, G, G, G};

			const v4sf invdistsqr = rcp(distsqr);
			const v4sf invdist = rcpsqrt(distsqr);

			ax += dx * gravfactor * invdistsqr * invdist;
			ay += dy * gravfactor * invdistsqr * invdist;
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
	pids[i] = A.id;
	pposx[i] = A.p.x;
	pposxe[i] = 0.0;
	pposy[i] = A.p.y;
	paccx[i] = 0.0;
	paccy[i] = 0.0;
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
	const int vcnt = (max % 4 == 0) ? max/4 : (max+3)/4;
	const v4sf factorv = (v4sf){factor, factor, factor, factor};

	for (int i=0; i < vcnt; i++){
//		obj.po = obj.p + (obj.po - obj.p) * factor;
		pvposox[i] = pvposx[i] + (pvposox[i] - pvposx[i]) * factorv;
		pvposoy[i] = pvposy[i] + (pvposoy[i] - pvposy[i]) * factorv;
	}
};


const void Physics::screenCollide(){
	const int max = objs.size();
	const real fixdiff = 1e-6;

	for (int i=0; i < max; i++){
		const real minx = boxmin.x + pradius[i];
		const real maxx = boxmax.x - pradius[i];
		const real minxfix = minx + fixdiff;
		const real maxxfix = maxx - fixdiff;

		if (pposx[i] > maxx){
//			po.x = p.x + max.x - po.x; p.x = max.x;
			pposox[i] = pposx[i] + maxxfix - pposox[i];
			pposx[i] = maxxfix;
		}
		if (pposx[i] < minx){
//			po.x = p.x + minx - po.x; p.x = minx;
			pposox[i] = pposx[i] + minxfix - pposox[i];
			pposx[i] = minxfix;
		}
	}

	for (int i=0; i < max; i++){
		const real miny = boxmin.y + pradius[i];
		const real maxy = boxmax.y - pradius[i];
		const real minyfix = miny + fixdiff;
		const real maxyfix = maxy - fixdiff;

		if (pposy[i] > maxy){
//			po.x = p.y + max.y - po.y; p.y = max.y;
			pposoy[i] = pposy[i] + maxyfix - pposoy[i];
			pposy[i] = maxyfix;
		}
		if (pposy[i] < miny){
//			po.y = p.y + miny - po.y; p.y = miny;
			pposoy[i] = pposy[i] + minyfix - pposoy[i];
			pposy[i] =  minyfix;
		}
	}
};


const void Physics::advanceTicks(int ticks){
	unpack();
	while (ticks > 0){
		updateAcceleration();
		updatePosition();
		screenCollide();
		const vector<idxpair>& cpairs = cdetect.generatePairs(objs.size());
		collidePairs(cpairs);
		ticks--;
	}
	pack();
};


const void Physics::collidePairs(const vector<idxpair>& pairs){
	const int max = pairs.size();
	for (int i=0; i < max; i++){
		const int Aidx = pairs[i].a;
		const int Bidx = pairs[i].b;
//		cout << Aidx << ":" << Bidx << endl;

		const real dr = pradius[Aidx] - pradius[Bidx];

		const real fixx = pposx[Bidx] - pposx[Aidx];
		const real fixy = pposy[Bidx] - pposy[Aidx];

		const real dist = sqrt(fixx*fixx + fixy*fixy);

		const real fixc = (1.0) * (1 + dr) / dist;

		const real fixca = fixc * prcpmss[Aidx];
		pposx[Aidx] -= fixx * fixca;
		pposy[Aidx] -= fixy * fixca;

		const real fixcb = fixc * prcpmss[Bidx];
		pposx[Bidx] += fixx * fixcb;
		pposy[Bidx] += fixy * fixcb;
	}
};


const void Physics::Shutdown(){
	FREE(pids);
	FREE(pposx);
	FREE(pposy);
	FREE(paccx);
	FREE(paccy);
	FREE(pmass);
	FREE(prcpmss);
	FREE(pradius);
	FREE(pposox);
	FREE(pposoy);
	copyPointers();
};


const void Physics::reallocate(){
	const unsigned long pidssz = (objs.size() > 4)  ? objs.size() * sizeof(unsigned long) : 16;
	pids = static_cast<unsigned int*>(arealloc(pids, pidssz));

	if (arrcnt >= objs.capacity()) return;

	const unsigned long count = objs.capacity();
	const unsigned long sz = (count > 16/sizeof(real)) ? count*sizeof(real) : 16;

	pposx = static_cast<areal*>(arealloc(pposx, sz));
	pposxe = static_cast<areal*>(arealloc(pposxe, sz));
	pposy = static_cast<areal*>(arealloc(pposy, sz));
	paccx = static_cast<areal*>(arealloc(paccx, sz));
	paccy = static_cast<areal*>(arealloc(paccy, sz));
	pmass = static_cast<areal*>(arealloc(pmass, sz));
	prcpmss = static_cast<areal*>(arealloc(prcpmss, sz));
	pradius = static_cast<areal*>(arealloc(pradius, sz));
	pposox = static_cast<areal*>(arealloc(pposox, sz));
	pposoy = static_cast<areal*>(arealloc(pposoy, sz));
	copyPointers();
	setExtras();

	arrcnt = objs.capacity();
};


const void Physics::copyPointers(){
	pvposx = reinterpret_cast<v4sf*>(pposx);
	pvposxe = reinterpret_cast<v4sf*>(pposxe);
	pvposy = reinterpret_cast<v4sf*>(pposy);
	pvaccx = reinterpret_cast<v4sf*>(paccx);
	pvaccy = reinterpret_cast<v4sf*>(paccy);
	pvmass = reinterpret_cast<v4sf*>(pmass);
	pvrcpmss = reinterpret_cast<v4sf*>(prcpmss);
	pvradius = reinterpret_cast<v4sf*>(pradius);
	pvposox = reinterpret_cast<v4sf*>(pposox);
	pvposoy = reinterpret_cast<v4sf*>(pposoy);
};


const void Physics::setExtras(){
	const int min = objs.size();
	const int max = objs.capacity();

	int i = min;
	while (i < max){
		pposx[i] = 0;
		pposy[i] = 0;
		pradius[i] = 1.0;
		pmass[i] = 0.0;
		i++;
	}
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
