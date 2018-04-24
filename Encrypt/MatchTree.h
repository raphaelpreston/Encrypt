#ifndef MATCHTREE_H
#define MATCHTREE_H
#include "Matchings.h"

typedef struct MatchTree {
	Match * match;	//match contained inside the node
	struct MatchTree * left;
	struct MatchTree * right;
} MatchTree;

/* inserts a given match into a node and puts it in the appropriate spot on the tree (send NULL as tree for creation) */
void mt_insert(MatchTree ** tree, Match * match);



















#endif
