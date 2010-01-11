#ifndef SHARED_H_
#define SHARED_H_

#include <iostream>
#include <string>
#include <cstdlib>


#if !defined(API_SDL) && !defined(API_GLUT)
#error "Neither glut or SDL specified, use -DAPI_GLUT or -DAPI_SDL"
#endif


#define GRAV_WINDOW_TITLE ("Grav")


using namespace std;


__attribute__ ((unused))
static const void fatalError(string msg){
	cerr << msg;
	exit(EXIT_FAILURE);
};


#endif /* SHARED_H_ */
