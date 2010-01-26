#ifndef SHARED_H_
#define SHARED_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>


#if !defined(API_SDL) && !defined(API_GLUT)
#error "Neither glut or SDL specified, use -DAPI_GLUT or -DAPI_SDL"
#endif


#define GRAV_WINDOW_TITLE ("Grav")
#define DEG2RAD(x) ((real(x)/180.0)*M_PI)
#define FRAND (real(rand())/real(RAND_MAX))
#define FREE(ptr) { if ((ptr) != NULL){ free(((void*)ptr)); (ptr) = NULL; } }

typedef float real;
typedef real areal __attribute__ ((__aligned__(16)));
typedef float v4sf __attribute__((vector_size(16)));


using namespace std;


__attribute__ ((unused))
static inline const void fatalError(string msg){
	cerr << msg;
	exit(EXIT_FAILURE);
};


__attribute__ ((unused))
static inline const long getticks(){
	static timespec time;
	if (clock_gettime(CLOCK_REALTIME, &time) < 0) return -1;
	else return time.tv_sec*1000 + ((time.tv_nsec/1000)/1000);
};


#ifndef _LP64
#include <cstring>
#include <malloc.h>
#endif

__attribute__ ((unused))
static inline void* arealloc(void* src, size_t sz){
	void* ptr;
	#ifdef _LP64
		ptr = realloc(src, sz);
		if (ptr == NULL) fatalError("Failed to reallocate memory!\n");
		return ptr;
	#else
		ptr = memalign(16, sz);
		if (ptr == NULL) fatalError("Failed to reallocate memory!\n");
		memcpy(ptr, src, sz);
		return ptr;
	#endif
};


__attribute__ ((unused))
static inline v4sf rcpsqrt(const v4sf x){
	return __builtin_ia32_rsqrtps(x);
};


__attribute__ ((unused))
static inline v4sf rcp(const v4sf x){
	return __builtin_ia32_rcpps(x);
};


__attribute__ ((unused))
static inline v4sf sqrt(const v4sf x){
	return __builtin_ia32_sqrtps(x);
};


__attribute__ ((unused))
static inline v4sf VMAX(v4sf a, v4sf b){
	return __builtin_ia32_maxps(a, b);
};


__attribute__ ((unused))
static inline v4sf VMIN(const v4sf a, const v4sf b){
	return __builtin_ia32_minps(a, b);
};


#endif /* SHARED_H_ */
