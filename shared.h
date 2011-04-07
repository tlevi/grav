#ifndef _SHARED_H
#define _SHARED_H
#define DEBUGPRINTS 0
#define DEBUGPRINT(PARAMS) {if (DEBUGPRINTS){ printf PARAMS; fflush(stdout); }}

#include <new>
#include <cmath>
using namespace std;

#include <SDL.h>

#define FRAND ((float)rand()/(float)RAND_MAX)

#define MAX(a,b)  ((a) > (b) ? (a) : (b))
#define MIN(a,b)  ((a) > (b) ? (b) : (a))


typedef unsigned short uShort;
typedef unsigned long uLong;
typedef unsigned int uInt;
typedef unsigned char uChar;


#endif
