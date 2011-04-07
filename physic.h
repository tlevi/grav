/*
// PHYSIC.H
*/

#ifndef _PHYSIC_H_
#define _PHYSIC_H_

#include <new>
#include <cmath>
using namespace std;
#include "vector.h"
#include <vector>


class physics{
	public:
		static float physTimeMultiplier;
		static uInt tickold;
		static uInt tick;
		static float td;
		static float otd;
		static float tdr;
		static float tdsqr;
		static int physicsCount;
		static int constraints;

	public:
		static const void updateTime(void);
		static const void updatePhysics(const uInt newtick);
		static const void satisfyConstraints(void);
		static const bool collideConstraintLoop(physics& phys, int k);
		static const bool collide(physics& A, physics& B);

	public:
		physics(float mass1);
		~physics(void);


		const inline void clearVelocity(void){
			op.x = np.x;
			op.y = np.y;
		};


	public:
		vector2 np;
		vector2 op;
		float invmass;
		float radius;
		vector2 a;
//		vector2 v;
		float mass;

	private:
		const void updatePosition(void);
		const void updateAcceleration(void);
};


extern vector<physics*> pPhysics;


#endif
