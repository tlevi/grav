/*
// PHYSIC.CPP
*/

#include "physic.h"
#include "main.h"
#include "object.h"
#include "render.h"


//physics * pPhysics[MAX_PHYSICS];
vector<physics*> pPhysics;

#define G (6.67e-11)


float physics::physTimeMultiplier = 10e3 / 4.0;
float physics::otd = physTimeMultiplier;
float physics::td = physTimeMultiplier;
float physics::tdsqr;
float physics::tdr;
int physics::physicsCount = 0;
int physics::constraints = 1;
uInt physics::tick = 0;


const void physics::updateAcceleration(void){
	vector2 ta(0);

	for (short k=0; k < physicsCount; ++k){
		if (pPhysics[k] == this) continue;

		const physics & B = *pPhysics[k];

		const float dx = B.np.x - np.x;
		const float dy = B.np.y - np.y;

		float c = sqrtf((dx * dx) + (dy * dy));
		c = G * B.mass / (c*c*c);

		ta.x += dx * c;
		ta.y += dy * c;
	}

	a.x = ta.x;
	a.y = ta.y;

	return;
}


const void physics::updatePosition(void){
	const vector2 t(np);

	np.x = np.x + (np.x - op.x) * tdr + a.x * tdsqr;
	op.x = t.x;

	np.y = np.y + (np.y - op.y) * tdr + a.y * tdsqr;
	op.y = t.y;

	return;
}


const void physics::updateTime(void){
	otd = td;
	td = physTimeMultiplier;
	tdr = td / otd;
//	inv2td = 1 / (td + otd);
	tdsqr = td * td;
	return;
}


//-Set initial values
//-Put a pointer in the physics array
physics::physics(float mass1){
	pPhysics.push_back(this);
	physicsCount++;
	op = vector2(0);
	np = vector2(0);
	a = vector2(0);
	mass = mass1;
	invmass = 1.0f / mass;
	radius = mass * (1.0f / 32.0f);
	return;
}


//-Find pointer in physics array
//-Move elements backwards one, sigh O(n) ;-(
physics::~physics(void){
	vector<physics*>::iterator it;
	for (it = pPhysics.begin(); it < pPhysics.end(); it++){
		if (*it == this) break;
	}

	if (it != pPhysics.end()){
		pPhysics.erase(it, it+1);
		physicsCount--;
	}

	return;
}


//do a 'bounce' if needed, fix position too
const void screenConstraint(physics * pThis){
	const float c1 = Global::screenWidth / 2 - pThis->radius;
	const float c2 = Global::screenHeight / 2 - pThis->radius;

	if (pThis->np.x > c1){
		pThis->op.x = pThis->np.x + c1 - pThis->op.x;
		pThis->np.x = c1;
	}
	else if (pThis->np.x < -c1){
		pThis->op.x = pThis->np.x - c1 - pThis->op.x;
		pThis->np.x = -c1;
	}

	if (pThis->np.y > c2){
		pThis->op.y = pThis->np.y + c2 - pThis->op.y;
		pThis->np.y = c2;
	}
	else if (pThis->np.y < -c2){
		pThis->op.y = pThis->np.y - c2 - pThis->op.y;
		pThis->np.y = -c2;
	}

	return;
}


//No, I can't remember how this works, sorry.
const bool physics::collideConstraintLoop(physics & phys, int k){
	bool bCollide = false;
	for (; k < physics::physicsCount; k++){
		physics& pK = *pPhysics[k];

		const float r1 = pK.radius;
		const float r2 = phys.radius;
		const vector2 fix = pK.np - phys.np;

		const float dist = SQRLENGTH(fix);
		const float bound = (r1 + r2) * (r1 + r2);
		if (dist > bound) continue;

		const float fixc = (1 + (r2 - r1)) / sqrtf(dist);

		const float pThisMul = fixc * phys.invmass;
		phys.np.x -= fix.x * pThisMul;
		phys.np.y -= fix.y * pThisMul;

		const float pKMul = fixc * pK.invmass;
		pK.np.x += fix.x * pKMul;
		pK.np.y += fix.y * pKMul;

		bCollide = true;
	}
	return bCollide;
}


const bool physics::collide(physics & A, physics & B){
	const float r1 = B.radius;
	const float r2 = A.radius;

	const vector2 fix = B.np - A.np;
	const float dist = SQRLENGTH(fix);
	const float bound = (r1 + r2) * (r1 + r2);

	if (dist > bound) return false;

	const float fixc = (1 + (r2 - r1)) / sqrtf(dist);

	const float fixca = fixc * A.invmass;
	A.np.x -= fix.x * fixca;
	A.np.y -= fix.y * fixca;

	const float fixcb = fixc * B.invmass;
	B.np.x += fix.x * fixcb;
	B.np.y += fix.y * fixcb;

	return true;
};


//Normally, gameloop just calls this once per-frame
//And that's all the interaction engine needs with physics
const void physics::updatePhysics(const uInt newtick){
	DEBUGPRINT(("%u | physics::updatePhysics\n", SDL_GetTicks()))

	vector<uInt> pairs;

	for (uInt t = tick; t < newtick; ++t){
		updateTime();
		updateTrails();

		//if (tree == NULL) tree = new PhysTree((void**) &pPhysics, physicsCount);
		//tree->objcnt = physicsCount;
		//tree->Build(vector2(Global::screenWidth, Global::screenHeight));

		//tree->runAllCollides();

		#pragma omp parallel for
		for (short i=0; i < physicsCount; i++) pPhysics[i]->updateAcceleration();

		for (short i=0; i < physicsCount; i++) pPhysics[i]->updatePosition();
		for (short i=0; i < physicsCount; i++) screenConstraint(pPhysics[i]);

		uLong count = 0;
		bool bCollide = true;
		while (bCollide == true){
			bCollide = false;
			for (short i=0; i < physicsCount; i++){
				if (collideConstraintLoop(*pPhysics[i], i+1)) bCollide = true;
			}
			if (++count > 100) break; //100 * O(n^2), ugly ;-)
		}
		//tree->runAllCollides();
		//tree->runCollides(pPhysics[0], (physNode*) tree->root);

		tick = t;
	}
	
	return;
}


//One-off, for object creatation or whatever
//The other main interaction point for every where else
const void physics::satisfyConstraints(void){
	for (uShort i=0; i < physicsCount; i++){
		screenConstraint(pPhysics[i]);
		collideConstraintLoop(*pPhysics[i], i+1);
	}
	return;
}
