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

typedef float real;
typedef real areal __attribute__ ((__aligned__(16)));


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
	#ifdef _LP64
		return realloc(src, sz);
	#else
		void* ptr = memalign(16, sz);
		if (ptr == NULL) return NULL;
		memcpy(ptr, src, sz);
		return ptr;
	#endif
};


#endif /* SHARED_H_ */
