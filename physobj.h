#ifndef PHYSOBJ_H_
#define PHYSOBJ_H_

#include "vector2.h"


class physobj{
public:
	friend class Physics;

	physobj();
	~physobj();

private:
	vector2 p;
	vector2 po;
	vector2 a;

	float mass;
	float radius;

	float invmass;
};


#endif /* PHYSOBJ_H_ */
