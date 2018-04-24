#include "Matchings.h"
#include "MatchTree.h"
//code inspired by https://www.thegeekstuff.com/2013/02/c-binary-tree/

//wait lol this will come later I have to balance the tree and stuff rip
//
//void mt_insert(MatchTree ** tree, Match * match) {
//	MatchTree * temp = NULL;
//	if (!(*tree)) {	//base case
//		temp = (MatchTree *)malloc(sizeof(MatchTree));
//		temp->left = temp->right = NULL;
//		temp->match = match;
//		*tree = temp;
//		return;
//	}
//
//	if (val < (*tree)->data) {
//		insert(&(*tree)->left, val);
//	}
//	else if (val >(*tree)->data) {
//		insert(&(*tree)->right, val);
//	}
//}