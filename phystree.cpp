#include "phystree.h"
#include "physic.h"
#include "main.h"


const vector2& PhysTree::getPosition(void * aLeaf){
	return  ((physics *) aLeaf)->np;
};


const void PhysTree::PostBuild(void){
//	DrawNode(Global::screen, root);
//	determineMass((physNode*) root);
//	determineMassCenter((physNode*) root);
	return;
};


const float PhysTree::determineMass(physNode * pNode){
	if (pNode->isLeafNode == true){
		pNode->mass = ((physics*)pNode->pLeaf)->mass;
		pNode->invmass = 1 / pNode->mass;
		return pNode->mass;
	}
	else{
		float sum = 0;
		for (int i=0; i<4; i++){
			if (pNode->children[i] != NULL){
				sum += determineMass((physNode*)pNode->children[i]);
			}
		}
		return sum;
	}
};


const vector2& PhysTree::determineMassCenter(physNode * pNode){
	const float totalmass = ((physNode*)root)->mass;
	if (pNode->isLeafNode == true){
		pNode->masscenter = ((physics*)pNode->pLeaf)->np;
	}
	else{
		pNode->masscenter = vector2(0,0);
		for (int i=0; i<4; i++){
			if (pNode->children[i] != NULL){
				const float c = pNode->mass / totalmass;
				pNode->masscenter += determineMassCenter((physNode*)pNode->children[i]) * c;
			}
		}
	}
	return pNode->masscenter;
};


const void PhysTree::runAllCollides(void){
	const int max = 128;
	for (int i=0; i < max; i++){
		bool runAgain = false;
		for (uInt i=0; i < objcnt; i++){
			const bool a = runCollides(objects[i], (physNode*)root);
			if (a == true) return;//runAgain = true;
		}
		if (runAgain == false) break;
	}
};


const bool PhysTree::runCollides(void * pPhys, physNode * pNode){
	if (pPhys == pNode->pLeaf) return false;

	if (pNode->isLeafNode == true){
		node * pNodea = Search(pPhys);
		const bool bCollide = physics::collide(* (physics*) pPhys, * (physics*) pNode->pLeaf);
		if (bCollide == true){
			//leaves have moved, rearrange tree
			void * aLeaf = pNodea->pLeaf;
			pNodea->pLeaf = NULL;
			void * bLeaf = pNode->pLeaf;
			pNode->pLeaf = NULL;
			CleanRemove(pNodea);
			DEBUGPRINT(("Finished first remove.\n"))
			const node * findParent = Search(pNode, root);
			if (findParent != NULL) CleanRemove(pNode);
			Insert(aLeaf);
			Insert(bLeaf);
			DEBUGPRINT(("Finished correcting tree.\n"))
		}
		return bCollide;
	}
	else{
		for (int i=0; i<4; i++){
			if (pNode->children[i] == NULL) continue;
			if (isTouchingNode(pPhys, (physNode*) pNode->children[i]) == true){
				const bool b = runCollides(pPhys, (physNode*) pNode->children[i]);
				if (b == true) return true;
			}
		}
		return false;
	}
};


/*	Tests if an object *touches* a node
	this includes if it is inside */
const bool PhysTree::isTouchingNode(void * pPhys, physNode * pNode){
	physics * pPhysics = (physics*) pPhys;

	const vector2 node_to_obj = vector2(abs(pPhysics->np.x - pNode->center.x), abs(pPhysics->np.y - pNode->center.y));
	const float r = pPhysics->radius;

	if (node_to_obj.x - r > pNode->size.y) return false;
	else if (node_to_obj.y - r > pNode->size.y) return false;
	else return true;
};
