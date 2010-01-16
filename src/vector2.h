#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <cmath>
#include "shared.h"


#define DOT(A,B)		(A.x*B.x+A.y*B.y)
#define NORMALIZE(A)	{A *= real(1.0) / sqrt(A.x*A.x+A.y*A.y);}
#define LENGTH(A)		(sqrt(A.x*A.x+A.y*A.y))
#define SQRLENGTH(A)	(A.x*A.x+A.y*A.y)
#define SQRDISTANCE(A,B) ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y))


class vector2 {
public:
	real x;
	real y;
	__attribute ((aligned (16)))


	explicit inline vector2(){};
	//If you want it zero'd when you start...try blah = vector3(0) (as per below)
	explicit inline vector2(const char zero) : x(0.0f), y(0.0f){};
	//If you want both values the same...
	explicit inline vector2(const real val) : x(val), y(val){};
	//Full init
	inline vector2(const real X, const real Y) : x(X), y(Y) {};


	inline void Normalise(void){
		const real l = 1.0 / sqrt(x*x + y*y);
		x *= l;
		y *= l;
	};


	const inline vector2 Normalised(void){
		const real l = 1.0 / sqrt(x*x + y*y);
		return vector2(x*l, y*l);
	};


	const inline real Length(void) const{
		return sqrt(x*x + y*y);
	};


	const inline real Dot(vector2& B) const{
		return x*B.x + y*B.y;
	};


	inline void operator += (const vector2& B){
		x += B.x;
		y += B.y;
	};


	inline void operator -= (const vector2& B){
		x -= B.x;
		y -= B.y;
	};


	inline void operator *= (const real f){
		x *= f;
		y *= f;
	};
	inline void operator *= (const vector2& B){
		x *= B.x;
		y *= B.y;
	};


	const inline vector2 operator - () const{
		return vector2(-x, -y);
	};
};


const inline vector2 operator * (const vector2& v, const real f){
	return vector2(v.x * f, v.y * f);
};


const inline vector2 operator + (const vector2& v1, const vector2& v2){
	return vector2(v1.x + v2.x, v1.y + v2.y);
};

const inline vector2 operator + (const vector2& v1, const real f){
	return vector2(v1.x + f, v1.y + f);
};


const inline vector2 operator - (const vector2& v1, const vector2& v2){
	return vector2(v1.x - v2.x, v1.y - v2.y);
};

const inline vector2 operator - (const vector2& v1, const real f){
	return vector2(v1.x - f, v1.y - f);
};


const inline vector2 operator * (const vector2& v1, const vector2& v2){
	return vector2(v1.x * v2.x, v1.y * v2.y);
};


#endif /* VECTOR2_H_ */
