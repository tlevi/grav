#ifndef PHYSICS_H_
#define PHYSICS_H_

using namespace std;

#include <vector>
#include "physobj.h"
#include "physcomn.h"


class Physics{
	friend class CDetect;

public:
	static const void Initialise();
	static const void Shutdown();

	/**
	 * Speed up or slow down the simulation time scale by the given factor
	 * @param factor factor to alter simulation time scale by
	 */
	static const void changeTimeScale(const real factor);

	static const void addObject(const real radius);
	static const void delObject(const physobj& oldobj);
	static const void delObject(const int idx);

	static inline const vector<physobj>& getObjs(){ return objs; };
	static inline const vector2& getBBmin(){ return boxmin; };
	static inline const vector2& getBBmax(){ return boxmax; };
	static inline const vector2& getBBsz(){ return boxsz; };

	/**
	 * Advance the simulation by 1 tick
	 **/
	static const void advanceTicks(const int ticks);

	static const void screenCollide();

private:
	static vector<physobj> objs;
	static vector2 boxmin;
	static vector2 boxmax;
	static vector2 boxsz;
	static real td;
	static real tdsqr;
	static bool objschanged;
	static int objcount;
	static CDetect& cdetect;

	/* pointers for SoA work */
	static unsigned int* __restrict pids;
	static areal* __restrict pposx;
	static areal* __restrict pposxe;
	static areal* __restrict pposy;
	static areal* __restrict paccx;
	static areal* __restrict paccy;
	static areal* __restrict pmass;
	static areal* __restrict prcpmss;
	static areal* __restrict pradius;
	static areal* __restrict pposox;
	static areal* __restrict pposoy;
	static v4sf* __restrict pvposx;
	static v4sf* __restrict pvposxe;
	static v4sf* __restrict pvposy;
	static v4sf* __restrict pvaccx;
	static v4sf* __restrict pvaccy;
	static v4sf* __restrict pvmass;
	static v4sf* __restrict pvrcpmss;
	static v4sf* __restrict pvradius;
	static v4sf* __restrict pvposox;
	static v4sf* __restrict pvposoy;
	static unsigned long arrcnt;

	static const void updateAcceleration();
	static const void updatePosition();
	static const void collidePairs(const vector<idxpair>& pairs);
	static const void copyPointers();
	static const void reallocate();
	static const void setExtras();
	static const void unpack();
	static const void pack();
};


#endif /* PHYSICS_H_ */
