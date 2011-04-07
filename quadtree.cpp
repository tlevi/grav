#include "quadtree.h"
#include "main.h"
#include <cstdlib>

/*	Build a QuadTree
	Each object in the array gets put into a leaf
	Then we add the leaves into the tree
	Creates the root node (initialises tree) if none exists
	Runs in O(nlogn)										*/
const void QuadTree::Build(const vector2& aSize){
	//DEBUGPRINT((stdout, "%u | QuadTree::Build\t\t(tree of %u objects)\n", SDL_GetTicks(), objcnt))
	//create tree if we dont have one
	if (root != NULL) delete root;
	root = makeNode();
	root->size = aSize;
	root->center = vector2(0, 0);
	root->depth = 0;

	for (uInt i = 0; i < objcnt; i++){
		if (objects[i] == NULL) continue;
		Insert(objects[i], root);
	}

	PostBuild();
};


const void QuadTree::PostBuild(void){
};


/*	QuadTree::Insert - insert a leaf into a node
	Runs in O(logn)								*/
const void QuadTree::Insert(void * aLeaf, node * pNode){
	DEBUGPRINT(("%u | QuadTree::Insert\t\t(%p, leaf %p)\n", SDL_GetTicks(), pNode, aLeaf))

	if (pNode->depth > 32){
		DEBUGPRINT(("%u | QuadTree::Insert\t\t(Depth >%u, %u)\n", SDL_GetTicks(), 32, pNode->depth));
		exit(1);
	}

	if (pNode->isLeafNode == false){
		const uInt indexNode = DetermineQuadrant(aLeaf, pNode);
		if (pNode->children[indexNode] != NULL){
			Insert(aLeaf, pNode->children[indexNode]);
		}
		else{ //node will have just this leaf so insert it on creation
			pNode->children[indexNode] = makeNode(aLeaf);
			pNode->children[indexNode]->depth = pNode->depth + 1;
			SetNodeSize(pNode->children[indexNode], pNode->center, pNode->size, indexNode);
		}
	}
	else if (pNode->pLeaf == NULL){
		pNode->leafNode(aLeaf);
	}
	else {
		void * oldLeaf = pNode->pLeaf;
		pNode->pLeaf = NULL;
		pNode->isLeafNode = false;

		const int indexNodeLeafNode = DetermineQuadrant(oldLeaf, pNode);//quadrant of node leaf
		pNode->children[indexNodeLeafNode] = makeNode(oldLeaf);//leaf into child node
		pNode->children[indexNodeLeafNode]->depth = pNode->depth + 1;
		SetNodeSize(pNode->children[indexNodeLeafNode], pNode->center, pNode->size, indexNodeLeafNode);

		const int indexLeafNode = DetermineQuadrant(aLeaf, pNode);//quadrant of new leaf
		if (indexLeafNode == indexNodeLeafNode) Insert(aLeaf, pNode->children[indexLeafNode]);
		else{
			pNode->children[indexLeafNode] = makeNode(aLeaf);
			pNode->children[indexLeafNode]->depth = pNode->depth + 1;
			SetNodeSize(pNode->children[indexLeafNode], pNode->center, pNode->size, indexLeafNode);
		}
	}
};


/*	QuadTree::DetermineQuadrant - return a child node *index* that leaf should go in */
const uInt QuadTree::DetermineQuadrant(void * aLeaf, node * pNode){
	const vector2 node_to_leaf = getPosition(aLeaf) - pNode->center;
	// 0 1   x is low bit
	// 2 3   y is high bit
	uInt flags = 0;
	if (node_to_leaf.x > 0) flags = 1;
	if (node_to_leaf.y < 0) flags |= (1<<1);
	return flags;
};


/*	QuadTree::DetermineQuadrant - return a child node that leaf should go in */
node * QuadTree::DetermineChildNode(void * aLeaf, node * pNode){
	const uInt t = DetermineQuadrant(aLeaf, pNode);
	return pNode->children[t];
};


/* Draw the tree, runs in O(nlogn) */
const void QuadTree::DrawNode(SDL_Surface * pSurf, node * pNode){
	SDL_Rect rect;
	rect.x = (short) (pNode->center.x - pNode->size.x/2 + root->size.x/2);
	rect.y = (short) (-pNode->center.y - pNode->size.y/2 + root->size.y/2);
	rect.w = (short) (pNode->size.x);
	rect.h = (short) (pNode->size.y);

	const uInt scale = 32;
	const uInt r = (pNode->depth < (256/scale)) ? pNode->depth * scale : 255;
	SDL_FillRect(pSurf, &rect, SDL_MapRGB(pSurf->format, r, 0, 0));

	if (pNode->isLeafNode == false){
		for (int i=0; i<4; i++){
			if (pNode->children[i] != NULL) DrawNode(pSurf, pNode->children[i]);
		}
	}
};


const void QuadTree::SetNodeSize(node *pNode, const vector2& pntCnt, const vector2& pntSz, const uInt quadrant){
	pNode->size = pntSz * 0.5;
	pNode->center = pntCnt;

	// 0 1   x is low bit
	// 2 3   y is high bit
	if (quadrant & (1<<0)) pNode->center.x += pNode->size.x * 0.5f;
	else pNode->center.x -= pNode->size.x * 0.5f;
	if (quadrant & (1<<1)) pNode->center.y -= pNode->size.y * 0.5f;
	else pNode->center.y += pNode->size.y * 0.5f;
};


/*	QuadTree::Search - return what node a leaf is in.
	Leaves not in the tree are still given a home,
	unless their quadrant(s) dont exist (returns NULL)
	Runs in O(logn)										*/
node * QuadTree::Search(void * aLeaf, node * checkNode){
	if (checkNode->isLeafNode == false){
		const uInt index = DetermineQuadrant(aLeaf, checkNode);
		if (checkNode->children[index] != NULL) return Search(aLeaf, checkNode->children[index]);
		else return NULL;
	}
	else return checkNode;
};


/*	QuadTree::Search - return what node a node is in.
	Nodes not in the tree are still given a home,
	unless their quadrant(s) dont exist (returns NULL)
	Runs in O(logn)										*/
node * QuadTree::Search(node * aNode, node * checkNode){
	DEBUGPRINT(("%u | QuadTree::Search\t\t(node %p in %p)\n", SDL_GetTicks(), aNode, checkNode))
	if (checkNode->isLeafNode == false){
		const uInt index = DetermineQuadrant(aNode, checkNode);
		if (checkNode->children[index] != NULL) return Search(aNode, checkNode->children[index]);
		else return NULL;
	}
	else return NULL;
};


const bool QuadTree::CleanRemove(node * aNode, node * checkNode){
	if (checkNode == root){
		DEBUGPRINT(("%u | QuadTree::CleanRemove\t(%p in %p)\n", SDL_GetTicks(), aNode, checkNode))
	}

	if (aNode == checkNode){
			delete this;
			return true;
	}
	else if (!checkNode->isLeafNode){
		for (int i=0; i < 4; i++){
			if (checkNode->children[i] != NULL){
				if (CleanRemove(aNode, checkNode->children[i])){
					checkNode->children[i] = NULL;
					break;
				}
			}
		}

		int child_count = 0;
		for (int i=0; i < 4; i++){
			if (checkNode->children[i] != NULL) child_count++;
		}

		if (child_count == 0) return true;
		else if (child_count == 1){

		}
		else return false;
	}
	else return false;

	return false;
};



const uInt QuadTree::DetermineQuadrant(node * aNode, node * checkNode){
	const vector2 checknode_to_node = aNode->center - checkNode->center;
	// 0 1   x is low bit
	// 2 3   y is high bit
	uInt flags = 0;
	if (checknode_to_node.x > 0) flags = 1;
	if (checknode_to_node.y < 0) flags |= (1<<1);
	return flags;
};


node * QuadTree::DetermineChildNode(node * aNode, node * checkNode){
	const uInt t = DetermineQuadrant(aNode, checkNode);
	return checkNode->children[t];
};
