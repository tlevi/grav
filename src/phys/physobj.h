#ifndef PHYSOBJ_H_
#define PHYSOBJ_H_

#include "../vector2.h"


class physobj{
public:
	friend class Physics;

	~physobj();

	const vector2& get_p() const { return p; };
	const float get_mass() const { return mass; };
	const float get_radius() const { return radius; };


private:
	physobj();
	physobj(const float radius);

	vector2 p;
	vector2 po;
	vector2 a;

	float mass;
	float radius;

	float invmass;
};


#endif /* PHYSOBJ_H_ */
