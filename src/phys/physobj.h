#ifndef PHYSOBJ_H_
#define PHYSOBJ_H_

#include "../vector2.h"


class physobj{
public:
	friend class Physics;

	const vector2& get_p() const { return p; };
	const real get_mass() const { return mass; };
	const real get_radius() const { return radius; };
	const unsigned int get_color() const { return color; };

	static unsigned int nextid;


private:
	physobj(const real radius);

	vector2 p;
	real mass;
	real radius;

	unsigned int id;
	unsigned int color;
};


#endif /* PHYSOBJ_H_ */
