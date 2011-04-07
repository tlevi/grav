#include "cdetect.h"


void CDetect::colliderPairs(vector<void*>& objs,  bool bChanged, vector<uInt>& pairs){
	pairs.clear();

	const int max = objs.size();
	for (int i=0; i < max; i++){
		for (int k=0; k < i; k++){
			pairs.push_back(i << 16 | k);
		}
	}

	for (uInt i=0; i < pairs.size(); i++) printf("%x\n", pairs[i]);
};


CDetect::CDetect(){
	pairs.reserve(512*512);
};


CDetect::~CDetect(){
};
