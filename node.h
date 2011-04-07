#ifndef _NODE_H_
#define _NODE_H_


#include "vector.h"


class node{
public:
	node * children[4];
	vector2 center;
	vector2 size;
	void * pLeaf;
	uInt depth;
	bool isLeafNode;

	node(void){
		pLeaf = NULL;
		depth = 0;
		isLeafNode = false;
		for (int i=0; i < 4; i++) children[i] = NULL;
		leafNode(NULL);
	};

	node(void * aLeaf){
		pLeaf = aLeaf;
		depth = 0;
		isLeafNode = false;
		for (int i=0; i < 4; i++){
			children[i] = NULL;
		}
		leafNode(aLeaf);
	};

	virtual ~node(void){
		for (int i=0; i<4; i++){
			if (children[i] != NULL){
				delete children[i];
				children[i] = NULL;
			}
		}
		delete[] &children;
	};

	const inline void leafNode(void * aLeaf){
		pLeaf = aLeaf;
		isLeafNode = true;
	};
};


#endif //_NODE_H_
