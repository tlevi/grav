#ifndef _PHYSTREE_H_
#define _PHYSTREE_H_


#include "quadtree.h"


class physNode: public node{
public:
	vector2 masscenter;
	float mass;
	float invmass;

	physNode(void) : node() {
		masscenter = vector2(0,0);
		mass = 0;
		invmass = 0;
	};
	physNode(void * aLeaf) : node(aLeaf) {
		masscenter = vector2(0,0);
		mass = 0;
		invmass = 0;
	};
	~physNode(void){};
};


class PhysTree: public QuadTree {
public:
	node * makeNode(void){ return new physNode(); };
	node * makeNode(void * aLeaf){ return new physNode(aLeaf); };

	PhysTree(void ** objs, uInt cnt) : QuadTree(objs, cnt){
	};
	const vector2& getPosition(void * aLeaf);

	const float determineMass(physNode * pNode);
	const vector2& determineMassCenter(physNode * pNode);
	const void runAllCollides(void);
	const bool runCollides(void * pPhysics, physNode * pNode);
	const bool isTouchingNode(void * pPhysics, physNode * pNode);
	const void PostBuild(void);
};


#endif //_PHYSTREE_H_
