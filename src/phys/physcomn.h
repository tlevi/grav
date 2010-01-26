#ifndef PHYSCOMN_H_
#define PHYSCOMN_H_


class idxpair{
public:
	int a;
	int b;
	idxpair(int A, int B) : a(min(A,B)), b(max(A,B)){};
};


class CDetect;
class Physics;


#endif /* PHYSCOMN_H_ */
