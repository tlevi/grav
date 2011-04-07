#ifndef _CDETECT_H_
#define _CDETECT_H_


#include "shared.h"
#include <vector>


class CDetect{
public:
	CDetect();
	~CDetect();
	virtual void colliderPairs(vector<void*>& objs, bool bChanged, vector<uInt>& pairs);

protected:
	vector<bool> pairs;
};


#endif
