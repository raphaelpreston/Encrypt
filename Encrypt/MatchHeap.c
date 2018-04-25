#include "MatchHeap.h"

Node * newNode(Match * match) {
	Node * node = (Node *)malloc(sizeof(Node));
	node->lChild = NULL;
	node->rChild = NULL;
	node->match = match;

	return node;
}

MatchHeap * newMatchHeap() {
	MatchHeap * heap = (MatchHeap *)malloc(sizeof(MatchHeap));
	heap->size = 0;
	heap->root = NULL;

	return heap;
}

void insertNode(MatchHeap * heap, Node * node) {
	/* first time adding a node */
	if (heap->size == 0) {
		heap->root = node;
	}
	else {
		//if(middle)
		//insertLeft(heap->root);
		//insertRight(heap->root);
	}
}

void insertLeft(Node * node) {	// 

}

void insertRight(Node * node) {

}