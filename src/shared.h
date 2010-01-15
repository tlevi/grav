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
#define DEG2RAD(x) ((float(x)/180.0)*M_PI)
#define FRAND (double(rand())/double(RAND_MAX))


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


#endif /* SHARED_H_ */
