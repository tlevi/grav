#ifndef _CDETECT_H
#define _CDETECT_H

#include "../shared.h"
#include "physobj.h"
#include "physcomn.h"
#include <vector>
#include <utility>


typedef unsigned int uInt;


class CDetect{
	friend class Physics;

	protected:
		virtual const vector<idxpair>& generatePairs(const int objcount);

		const bool isListChanged(const int objcount);

		vector<uInt> ids;
		vector<idxpair> pairs;
};


#endif /* _CDETECT_H */
