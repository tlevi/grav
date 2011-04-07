#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <cmath>
#include "shared.h"

namespace std {


#define DOT(A,B)		(A.x*B.x+A.y*B.y)
#define NORMALIZE(A)	{A *= 1.0 / sqrtf(A.x*A.x+A.y*A.y);}
#define LENGTH(A)		(sqrtf(A.x*A.x+A.y*A.y))
#define SQRLENGTH(A)	(A.x*A.x+A.y*A.y)
#define SQRDISTANCE(A,B) ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y))


class vector2{
public:
	struct{
		float x;
		float y;
	};

	explicit inline vector2(){};

	//If you want it zero'd when you start...try blah = vector3(0) (as per below)
	inline vector2(const float z) : x(0.0f), y(0.0f){};

	//Full init of each var :)
	inline vector2(const float X, const float Y) : x(X), y(Y) {};

	inline void Normalise(void){
		const float l = 1.0 / sqrtf(x*x + y*y);
		x *= l;
		y *= l;
	};

	const inline float Length(void) const{
		return sqrtf(x*x + y*y);
	};

	const inline float Dot(vector2& B) const{
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

	inline void operator *= (const float f){
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

	const inline friend vector2 operator + (const vector2& v1, const vector2& v2){
		return vector2(v1.x + v2.x, v1.y + v2.y);
	};

	const inline friend vector2 operator - (const vector2& v1, const vector2& v2){
		return vector2(v1.x - v2.x, v1.y - v2.y);
	};

	const inline friend vector2 operator * (const vector2& v1, const vector2& v2){
		return vector2(v1.x * v2.x, v1.y * v2.y);
	};

	const inline friend vector2 operator * (const vector2& v, const float f){
		return vector2(v.x * f, v.y * f);
	};
};

}

#endif
