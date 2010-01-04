#ifndef PHYSICS_H_
#define PHYSICS_H_

using namespace std;

#include <vector>
#include "physobj.h"


class Physics{
public:
	static vector<physobj> objs;
	static int objcount;
	static bool objschanged;


	/**
	 * Speed up or slow down the simulation time scale by the given factor
	 * @param factor factor to alter simulation time scale by
	 */
	static const void changeTimeScale(const float factor);


	static const void addObject(const float radius);

	static const void delObject(const physobj& oldobj);
	static const void delObject(const int idx);

	static inline const vector<physobj>& getObjs(){ return objs; };


	/**
	 * Advance the simulation by 1 tick
	 **/
	static const void advanceTick();


private:
	static float td;
	static float tdsqr;


	static const void updateAcceleration();
	static const void updatePosition();
};


#endif /* PHYSICS_H_ */
