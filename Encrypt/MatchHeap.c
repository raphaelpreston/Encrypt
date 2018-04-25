#include "MatchHeap.h"
#include <stdio.h>
#include <stdlib.h>

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
	if (node->match == NULL) printf("ERROR: NODE'S MATCH WAS NULL\n\n.");

	/* first time adding a node */
	if (heap->root == NULL) {
		heap->root = node;
	}
	else {
		if (middle(node->match, 1) < middle(heap->root->match, 1)) insertRecurse(heap->root, node);	//node to add belongs to left
		else if (middle(node->match, -1) > middle(heap->root->match, -1)) insertRecurse(heap->root, node);	//node to add belongs to right
		else insertRecurse(heap->root, node);	//the share the same middle, insert left
	}
}

void insertRecurse(Node * root, Node * node) {

	/* if it's bigger than the current one, swap the root out for the node */
	if (matchLength(node->match) > matchLength(root->match)) {
		swap(node, root);	//values switch pointers
	}

	/* send the appropriate node down */
	if (middle(node->match, 1) < middle(root->match, 1)) {
		if (!root->lChild) {
			root->lChild = node;
			return;
		}
		else {
			insertRecurse(root->lChild, node);	//node to add belongs to left
		}
	}
	else if (middle(node->match, -1) > middle(root->match, -1)) {
		if (!root->rChild) {
			root->rChild = node;
			return;
		}
		else {
			insertRecurse(root->rChild, node);	//node to add belongs to right
		}
	}
	else {	//ties go to the left
		if (!root->lChild) {
			root->lChild = node;
			return;
		}
		else {
			insertRecurse(root->lChild, node);	//node to add belongs to left
		}
	}
}

void printHeap(MatchHeap * heap) {
	if (heap == NULL || heap->root == NULL) return;

	printNodeRecurse(heap->root);
}

void printNodeRecurse(Node * node) {
	if (!node) return;

	for (int i = 0; i < middle(node->match, -1) ; i++) {
		printf(" ");
	}
	printMatch(node->match);
	printf("\n");

	printNodeRecurse(node->lChild);
	printNodeRecurse(node->rChild);
	
}