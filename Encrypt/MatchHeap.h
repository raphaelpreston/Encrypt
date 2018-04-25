//code based of off Robin Thomas's GitHub version
#ifndef MATCHHEAP_H
#define MATCHHEAP_H

#include "Matchings.h"

/* properties of this heap:
1. A parent match will always have a bigger length
2. A parent's left child's middle is to the left or equal to itself
3. A parent's right child's middle is to the right of itself



*/
typedef struct Node {
	Match * match;
	struct Node * lChild;
	struct Node * rChild;
} Node;

typedef struct MatchHeap {
	int size;
	struct Node * root;
} MatchHeap;

MatchHeap * newMatchHeap();

/* percolate down */
void insertNode(MatchHeap * heap, Match * match);

/* updates node and calls heapify down and heapify up */
void updateNode();

/* heapify down */

/* heapfiy up */














#endif
