#ifndef SHARED_H_
#define SHARED_H_


#if !defined(API_SDL) && !defined(API_GLUT)
#error "Neither glut or SDL specified, use -DAPI_GLUT or -DAPI_SDL"
#endif


using namespace std;


typedef unsigned int uInt;
typedef unsigned long uLong;
typedef unsigned short uShort;
typedef unsigned char uChar;


#endif /* SHARED_H_ */
