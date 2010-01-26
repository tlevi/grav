#include "CDetect.h"
#include "Physics.h"
#include <iostream>


const vector<idxpair>& CDetect::generatePairs(const int objcount){
//	if (!isListChanged(objcount)) return pairs;

	const real* const pposx = Physics::pposx;
	const real* const pposy = Physics::pposy;
	const real* const pradius = Physics::pradius;

	pairs.clear();

	for (int i=0; i < objcount; i++){
		for (int k=0; k < objcount; k++){
			const real dx = pposx[i] - pposx[k];
			const real dy = pposy[i] - pposy[k];
			const real dmax = pradius[i] + pradius[k];
			if (dx*dx+dy*dy <= dmax*dmax && k > i) pairs.push_back(idxpair(i, k));
		}
	}

	return pairs;
};


const bool CDetect::isListChanged(const int sz_objs){
	const int sz_ids = ids.size();
	if (sz_objs != sz_ids) return true;

	const unsigned int* const pids = Physics::pids;
	for (int i=0; i < sz_ids; ++i){
		if (ids[i] != pids[i]) return true;
	}

	return false;
};
