#ifndef _QUADTREE_H_
#define _QUADTREE_H_


#include "node.h"


class QuadTree{
public:
	node * root;
	void ** objects;
	uInt objcnt;

	QuadTree(){
		root = NULL;
	};
	QuadTree(void ** objs, uInt cnt){
		root = NULL;
		objcnt = cnt;
		objects = objs;
	};

	virtual const void PostBuild(void);
	virtual node * makeNode(void){ return new node(); };
	virtual node * makeNode(void * aLeaf){ return new node(aLeaf); };
	virtual const vector2& getPosition(void * aLeaf){
		static vector2 zero = vector2(0,0);
		return zero;
	};

	const void Build(const vector2& aSize);
	const void Insert(void * aLeaf){ Insert(aLeaf, root); };
	const void Insert(void * aLeaf, node * pNode);
	const bool Remove(node * pNode, node * checkNode);

	const bool CleanRemove(node * aNode, node * checkNode);
	const void CleanRemove(node * aNode){ CleanRemove(aNode, root); };
	node * Search(void * aLeaf, node * checkNode);
	node * Search(void * aLeaf){ return Search(aLeaf, root); };
	node * Search(node * aNode, node * checkNode);
	node * Search(node * aNode){ return Search(aNode, root); };
	const void SetNodeSize(node * pNode, const vector2& pntCnt, const vector2& pntSz, const uInt quadrant);
	const void DrawNode(SDL_Surface * pSurf, node * pNode);
	const uInt DetermineQuadrant(void * aLeaf, node * pNode);
	const uInt DetermineQuadrant(node * aNode, node * checkNode);
	node * DetermineChildNode(void * aLeaf, node * pNode);
	node * DetermineChildNode(node * aNode, node * checkNode);
};


#endif //_QUADTREE_H_
