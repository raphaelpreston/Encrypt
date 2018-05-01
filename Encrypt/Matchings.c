#include <stdio.h>

#include "Matchings.h"
#include "Binary.h"


#define PRINT 0

Match * newMatch(int b, int c, int l, int type) {
	/* allocate space for match boye */
	Match * m;
	m = (Match *)malloc(sizeof(Match));
	if (m) {

		/* fill boye */
		m->start = b;
		m->end = b + l - 1;
		m->cindex = c;
		m->lChild = NULL;
		m->rChild = NULL;
		m->parent = NULL;
		m->type = type;

		if (l < 2) {
			printf("\nWarning: length of match "); printMatch(m); printf(" is %i\n", l);
		}

		return m;
	}
	else {
		printf("Failed to malloc space for a new match (b: %i, c: %i, l: %i)\n", b, c, l);
		return NULL;
	}
}

Matches * newMatches(int num_bits) {

	/* allocate space */
	Matches * m = (Matches *)malloc(sizeof(Matches));
	if (m) {

		/* make both arrays */
		Match ** start;
		Match ** end;
		start = (Match **)calloc(num_bits, sizeof(Match *));	//technically we can remove 1 because there are no endings at 0 
		end = (Match **)calloc(num_bits, sizeof(Match *));	// use calloc to initialize each match * to 0

															/* make the array to keep track of matched bits */
		bool * used = (bool *)calloc(num_bits, sizeof(bool));

		if (start && end) {

			/* fill m */
			m->start_arr = start;
			m->end_arr = end;
			m->size = num_bits;
			m->num_matches = 0;
			m->used = used;
			m->heap = newMatchHeap();
		}
		else {
			printf("Failed to calloc space for begin and end array with %i bits each", num_bits);
			return NULL;
		}
		return m;
	}
	else {
		printf("Failed to malloc space for new matches object.");
		return NULL;
	}
}

int matchLength(Match * m) {
	return m->end - m->start + 1;
}

void addMatch(Matches * matches, Match * m) {
	if (PRINT == 1) { printf("Attempting to add match: "); printMatch(m); printf("\n"); }
	int length = matchLength(m);
	Match * curr;

	/* error check for exceeding limit of arrays */
	if (m->end >= matches->size || m->start < 0) {
		printf("\nMatch "); printMatch(m); printf(" out of bounds of matches arrays.\n");
	}


	/* get the max compatable match from all max matches that start in range m_start to m_end + 1 and all that end in start - 1 to end (that match in both crypt and body)*/

	if (!matches->num_matches == 0) {	//only look for max matches if the arrays are not empty
										//printf("\nStart_arr: ");
		Match * max_start = maxMatchInRange(matches->start_arr, m, m->start, m->end + 1 >= matches->size ? m->end : m->end + 1);	//make sure the upper limit is in bounds
																																	//printf("\nEnd arr:");
		Match * max_end = maxMatchInRange(matches->end_arr, m, m->start - 1 < 0 ? m->start : m->start - 1, m->end);		//make sure the lower limit is in bounds

		if (max_start != NULL) {
			if (PRINT == 1) { printf("The max start matching found in range [%i,%i] was ", m->start, m->end + 1 >= matches->size ? m->end : m->end + 1); printMatch(max_start); printf("\n"); }
		}
		else {
			if (PRINT == 1){ printf("No max start match found in range [%i,%i]", m->start, m->end + 1 >= matches->size ? m->end : m->end + 1); printf("\n");}
		}

		if (max_end != NULL) {
			if (PRINT == 1) { printf("The max end matching found in range [%i,%i] was ", m->start - 1 < 0 ? m->start : m->start - 1, m->end); printMatch(max_end); printf("\n"); }
		}
		else {
			if (PRINT == 1) { printf("No max end match found in range [%i,%i]", m->start - 1 < 0 ? m->start : m->start - 1, m->end); printf("\n"); }
		}

		// merge the 0/2/3 to get a supermatch
		if (max_start || max_end) {	//merge() can handle if one is NULL but no reason to call if they both are
			Match * max_matches[] = { m, max_start, max_end };
			Match * merged = merge(max_matches, 3);
			if (merged != NULL) {
				if (PRINT == 1) { printf("New merged baby match: "); printMatch(merged); printf(" \n"); }

				// delete the 1/2 out of both arrays
				if (max_start == max_end) {	//protect against double delete
					if (max_start) {
						if (PRINT == 1) { printf("Attempting to delete the max_start: "); printMatch(max_start); printf("\n"); }
						deleteMatch(matches, max_start);
					}
				}
				else {
					if (max_start) {
						if (PRINT == 1) {printf("Attempting to delete the max_start: "); printMatch(max_start); printf("\n");}
						deleteMatch(matches, max_start);
					}
					if (max_end) {
						if (PRINT == 1) { printf("Attempting to delete the max_end: "); printMatch(max_end); printf("\n"); }
						deleteMatch(matches, max_end);
					}
				}

				// now we are adding the supermatch, so m = supermatch
				m = merged;
				if (PRINT == 1) { printf("Now adding merged match: "); printMatch(m); printf("\n"); }
			}
			else {
				if (PRINT == 1) printf("The merge was unsuccsesful because they came up with different body/crypt lengths.\n");
			}

		}
		else {
			if (PRINT == 1) printf("No attempt to merge because max start and max end were both NULL.\n");
		}


		if (PRINT == 1) printf("Attempting to merge:\n");
	}
	else {
		if (PRINT == 1) printf("Matches were empty so no merging was attempted.\n");
	}

	if (PRINT == 1) { printf("Adding match: "); printMatch(m); printf("\n"); }
	length = matchLength(m); //update length

	/* add to start_arr */
	int start = m->start;
	// bool used = false;
	if (matches->start_arr[start] == NULL) {	//first match * in this linked list
		matches->start_arr[start] = m;
	}
	else {

		/* keep max length in front */
		curr = matches->start_arr[start];	//the max
		if ((curr->end - curr->start + 1) < length) {	//if the match to add is bigger than the first one, replace it and free the old one
			// free(curr);
			matches->start_arr[start] = m;
			// used = true;
		}
	}

	/* repeat with end_arr*/
	int end = m->end;
	if (matches->end_arr[end] == NULL) {	//first match * in this linked list
		matches->end_arr[end] = m;
	}
	else {

		/* keep max length in front */
		curr = matches->end_arr[end];
		if ((curr->end - curr->start + 1) < length) {	//if the match to add is bigger than the first one, replace it and free old one
			// free(curr);
			matches->end_arr[end] = m;
		}
		else {
			//if (!used) free(m);	//wasn't used in start array so we can trash it
		}
	}

	matches->num_matches++;			//added one match to the matches

	if (PRINT == 1) printf("Match adding process was completed (attempted).\n");
}

void printMatch(Match * m){
	printf("(%i-%i,%i-%i [%i,%s] - %.1f - %p [P: %p, LC: %p, RC: %p])", m->start, m->end, m->cindex, m->cindex + m->end - m->start, matchLength(m), m->type == 1 ? "+" : "-", middle(m), m, m->parent, m->lChild, m->rChild);
}

void printValidity(Match * m, Binary * b) {
	bool valid = true;
	int fail;
	int conflict = false;

	int bi;
	int ci;

	for (int i = 0; i + m->start <= m->end; i++) {	//test each reference point
		bi = b->body->arr[m->start + i];
		ci = b->crypt->arr[m->cindex + i];

		if ((bi != ci && m->type == 1) || (bi == ci && m->type == 2)) {
			if (valid) fail = m->start + i;	//assign it on first fail but not again
			valid = false;
		}
	}

	/* check its children */
	if (m->lChild && m->lChild->parent != m) conflict = true;
	if (m->rChild && m->rChild->parent != m) conflict = true;

	/* check its parent */
	if (m->parent && m->parent->lChild != m && m->parent->rChild != m) conflict = true;

	printf("(%i-%i,%i-%i [%i,%s] ... %s and %s)", m->start, m->end, m->cindex, m->cindex + m->end - m->start, matchLength(m), m->type == 1 ? "+" : "-", valid ? "VALID" : "NOT VALID", conflict ? "CONFLICTED" : "NO CONFLICT");

}

void printMatchesValidity(Matches * m, Binary * b) {
	Match ** start = m->start_arr;
	Match ** end = m->end_arr;

	printf("Start Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (start[i] != NULL) {
			printf(" "); printValidity(start[i], b); printf("\n");
		}
	}

	printf("End Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (end[i] != NULL) {
			printf(" "); printValidity(end[i], b); printf("\n");
		}
	}
	printf("Num matches: %i\n", m->num_matches);

}

void printMatches(Matches * m) {
	Match ** start = m->start_arr;
	Match ** end = m->end_arr;

	printf("Start Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (start[i] != NULL) {
			printf(" "); printMatch(start[i]); printf("\n");
		}
	}

	printf("End Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (end[i] != NULL) {
			printf(" "); printMatch(end[i]); printf("\n");
		}
	}
	printf("Num matches: %i\n", m->num_matches);
}

Match * maxMatchInRange(Match ** arr, Match * m, int start, int end) {
	Match * max;
	Match * curr;
	max = arr[m->start] == NULL || arr[m->start] != m->type || !cryptCompatable(m, arr[m->start]) ? NULL : arr[m->start];	//we are trying to find the max compatable pair to merge (must test to make sure it's not NULL first and that its the same type
	if (PRINT == 1)  printf("\n");
	for (int i = start; i <= end; i++) {		//must retest for some reason i dunno why lol
		curr = arr[i];	//current max match
		if (curr != NULL && m->type == curr->type && cryptCompatable(m, curr)) {	//have to test to make sure it's not null otherwise cryptComp will throw an error (also make sure it's crypt comp and same type)
			if (curr != NULL) {
				if (PRINT == 1) { printf("Comparing current: "); printMatch(curr); printf(" against max: "); if (max == NULL)printf("NULL"); else printMatch(max); printf("\n"); }
				if (PRINT == 1) { if (!max == NULL) printf("Is %i > %i?\n", curr->end - m->start, max->end - m->start); }
				if (max == NULL) {

					if (max == NULL || testMerge(curr, m) > testMerge(max, m)) {
						max = curr;
						if (PRINT == 1) printf("Yes, assigned.\n");
					}
				}
				else if(PRINT == 1) printf("No, skipped.\n");
			}
			else {
				if (PRINT == 1) printf("Didn't execute because curr is %s and max_start is %s\n", curr == NULL ? "NULL" : "NOT NULL", max == NULL ? "NULL" : "NOT NULL");
			}
		}
		else {
			if (PRINT == 1) printf("The two matches weren't compatable or one was NULL.\n");
		}
	}
	return max;
}

bool cryptCompatable(Match * a, Match * b) {
	/* compatable means that the crypt portions can be merged */
	Match * m1;
	Match * m2;
	if (a->cindex <= b->cindex) {	//sort them accordingly
		m1 = a;
		m2 = b;
	}
	else {
		m1 = b;
		m2 = a;
	}

	int m1_end = m1->cindex + matchLength(m1) - 1;
	int m2_end = m2->cindex + matchLength(m2) - 1;
	
	/* the crypt portions can be merged if there is no more than 1 difference between them */
	if (m2->cindex - m1_end > 1) return false;
	
	/* also return false if the sides aren't aligned */
	if (!(m2->start >= m1->start && m2->cindex >= m1->cindex) || !(m2->start <= m1->start && m2->cindex <= m1->cindex)) return false;

	return true;
}

int testMerge(Match * a, Match * b) {
	Match * m1;
	Match * m2;
	if (a->start <= b->start) {	//sort them accordingly
		m1 = a;
		m2 = b;
	}
	else {
		m1 = b;
		m2 = a;
	}

	/* must be within 1 of eachother */
	if (m2->start - m1->end > 1) return 0;

	/* test to see if one completely envelopes the other */
	if (m2->start >= m1->start && m2->end <= m1->end) return matchLength(m1);  //m2 in m1
	if (m1->start >= m2->start && m1->end <= m2->end) return matchLength(m2); //m1 in m2

	return m2->end - m1->start + 1;
}

Match * merge(Match * matches[], int size) {	//will return NULL if merge made unequal body/crypt match.
												// IF THEY AREN'T COMPATABLE U MESSED UP
	Match * match;

	int min_b_start = matches[0]->start;
	int max_b_end = matches[0]->end;
	int min_c_start = matches[0]->cindex;
	int max_c_end = matches[0]->cindex + matchLength(matches[0]) - 1;	//matches[0] can't be NULL

	for (int i = 1; i < size; i++) {	//take the least start and the greatest end
		if (matches[i] != NULL) {
			//type check
			if (matches[i]->type != matches[0]->type)printf("WARNING: MERGING MATCHES OF DIFFERENT TYPES!!\n\n");
			// min/max for body
			if (matches[i]->start < min_b_start) min_b_start = matches[i]->start;
			if (matches[i]->end > max_b_end) max_b_end = matches[i]->end;

			//min/max for crypt
			if (matches[i]->cindex < min_c_start) min_c_start = matches[i]->cindex;
			if (matches[i]->cindex + matchLength(matches[i]) - 1 > max_c_end) max_c_end = matches[i]->cindex + matchLength(matches[i]) - 1;
		}
	}

	/* do the real merging */
	if (PRINT == 1) printf("\n(%i, %i, %i)", matches[1]->start, matches[1]->cindex, matches[1]->end - matches[1]->start + 1);
	if (PRINT == 1) printf("\nmin_b_start: %i\nmax_b_end: %i\nmin_c_start: %i\nmax_c_end:%i",  min_b_start, max_b_end, min_c_start, max_c_end);
	if (max_b_end - min_b_start != max_c_end - min_c_start) return NULL;	//when all merged together, the length of the body match must be equal to the length of the crypt match

	match = newMatch(min_b_start, min_c_start, max_b_end - min_b_start + 1, matches[0]->type);
	return match;
}

bool deleteMatch(Matches * matches, Match * match) {
	if (matches->size == 0) return false;
	Match * curr;
	Match * temp;

	//delete from start_arr by finding index of start
	curr = matches->start_arr[match->start];
	if (curr == match) {	//simply remove head
		temp = matches->start_arr[match->start];
	}
	else {
		return false; //this shouldn't happen
	}

	//repeat with end_arr
	curr = matches->end_arr[match->end];
	if (curr == match) {	//simply reassign head
		free(matches->end_arr[match->end]);
	}
	else {
		// delete it
		return false;		//this shouldn't happen
	}
	// free(match); doesn't work lol
	matches->num_matches--;
	return true;
}

int modifiedMatchLength(Match * m, int start, int end) {	//modified match length 
	if (m->end <= start || m->start >= end) return matchLength(m); //completely outside limits
	if (m->start >= start && m->end <= end) return 0;	//enveloped or exactly same length
	if (m->start >= start && m->start < end && m->end > end) return matchLength(m) - (end - m->start);	//starts inside and ends to the right
	if (m->start < start && m->end <= end && m->end > start) return matchLength(m) - (m->end - start);	//starts to the left and ends inside
	else printf("YOU DIDN'T ACCOUNT FOR SOMETHING LMAO\n\n\n\n\n");
}

double middle(Match * m) {
	double length = (double)matchLength(m);
	return m->start + length / 2.0;
}

void swapValues(Match * a, Match * b) {
	Match temp = *a;

	// assign all b stuff to a
	a->start = b->start;
	a->end = b->end;
	a->cindex = b->cindex;
	a->type = b->type;
	a->lChild = b->lChild;
	a->rChild = b->rChild;
	a->parent = b->parent;

	//assign all a stuff to b
	b->start = temp.start;
	b->end = temp.end;
	b->cindex = temp.cindex;
	b->type = temp.type;
	b->lChild = temp.lChild;
	b->rChild = temp.rChild;
	b->parent = temp.parent;

	//printf("Post swap:         "); printMatch(a); printf(" and "); printMatch(b); printf("\n");
}

//void deleteMatches(matches * m, int num_bits) {
//	for (int i = 0; i < num_bits; i++) {
//
//	}
//	free(m->begin_arr);
//	free(m->end_arr);
//}


/* MATCHHEAP */

MatchHeap * newMatchHeap() {
	MatchHeap * heap = (MatchHeap *)malloc(sizeof(MatchHeap));
	heap->size = 0;
	heap->root = NULL;

	return heap;
}

void heap_insertMatch(Matches * matches, Match * match) {
	if (match == NULL) printf("ERROR: MATCH WAS NULL\n\n.");
	printf("\nInserting: "); printMatch(match); printf("\n");

	/* first time adding a node */
	if (matches->heap->root == NULL) {
		printf("Heap root is NULL so adding "); printMatch(match); printf(" as the root.\n");
		match->parent = NULL;
		matches->heap->root = match;
	}
	else {
		printf("Heap wasn't null so calling percolating down, starting at root: "); printMatch(matches->heap->root); printf("\n");
		heap_insertRecurse(matches->heap, matches->heap->root, match);
	}
}

void heap_insertRecurse(MatchHeap * heap, Match * root, Match * match) {	//have to send heap in case we are replacing the root, send it as null if it's not needed to check
	printf("Comparing match: "); printMatch(match); printf(" with root: "); printMatch(root); printf("\n");

	/* if it's bigger than the current one, insert it in as a new root of the mini tree */
	if (matchLength(match) > matchLength(root)) {
		/* reassign root of matches heap if neccesary */
		if (heap && heap->root == root) heap->root = match;

		printf("Length of match: "); printMatch(match); printf(" is bigger than root: "); printMatch(root); printf(".. commencing root replacement.\n");
		
		/* make the match become the root of the new mini tree */
		bool wentToLeft = rootReplace(root, match);
		
		/* if the root went to the left of the new match, have to re-add stuff to the right of the old root	starting at the new root (match) & vice versa */
		if (wentToLeft) {
			if(root->rChild) reAdd(root->rChild, match);
		}
		else {
			if (root->lChild) reAdd(root->lChild, match);
		}

		return;
	}
	
	/* assign match or continue percolating down */

	if (goesToLeft(match, root)) {
		if (!root->lChild) {	//no left child yet, assign
			root->lChild = match;
			match->parent = root;
			return;
		}
		else {	//has a left child
			heap_insertRecurse(NULL, root->lChild, match);	//send it further down the tree to the left
		}
	}
	else {	//goes to the right
		if (!root->rChild) {	//no right chid yet, assign
			root->rChild = match;
			match->parent = root;
			return;
		}
		else {	//has a right child
			heap_insertRecurse(NULL, root->rChild, match); //send it further down the tree to the right
		}
	}
}

bool rootReplace(Match * root, Match * match) {	//returns true if root went to left of match, false otherwise

	/* reassign the old root's parent's child pointer */
	if (root->parent) {	//true root won't have a parent
		if (root == root->parent->lChild) root->parent->lChild = match;
		else root->parent->rChild = match;
	}

	/* make new match's parent the old root's parent */
	match->parent = root->parent;

	/* update the old root's parent to the new match */
	root->parent = match;

	
	/* appropriately assign the old root as the child of the new match */
	if (goesToLeft(root, match)) {
		match->lChild = root;
		return true;
	} else {
		match->rChild = root;
		return false;
	}
}

bool goesToLeft(Match * child, Match * parent) {
	if (middle(child) < middle(parent)) {
		printf("Middle of match "); printMatch(child); printf(" was to the left of the middle of the root "); printMatch(parent); printf("\n");
		return true;	//goes to left
	}
	else if (middle(child) > middle(parent)) {
		printf("Middle of match "); printMatch(child); printf(" was to the right of the middle of the root "); printMatch(parent); printf("\n");
		return false;	//goes to right
	}
	else {
		printf("Middle of match "); printMatch(child); printf(" was to the left of the middle of the root "); printMatch(parent); printf("\n");
		return true;	//ties go left
	}
}

void reAdd(Match * root, Match * dest) {

	/* cut off the connection to the parent */
	if (root->parent) {	//true root won't have a parent
		if (root == root->parent->lChild) root->parent->lChild = NULL;
		else root->parent->rChild = NULL;
		root->parent = NULL;
	}

	/* make a copy of lChild and rChild and cut of connections */
	Match * lChild = root->lChild;
	Match * rChild = root->rChild;
	root->lChild = NULL;
	root->rChild = NULL;

	/* insert the newly cut off root to the destination */
	heap_insertRecurse(NULL, dest, root);

	/* readd rest of minitree */
	if (lChild) reAdd(lChild, dest);
	if (rChild) reAdd(rChild, dest);

	return;
}

void printHeap(MatchHeap * heap) {
	if (heap == NULL || heap->root == NULL) return;
	printNodeRecurse(heap->root);
}

void printNodeRecurse(Match * match) {
	if (!match) return;

	for (int i = 0; i < middle(match); i++) {
		printf(" ");
	}
	printMatch(match); printf("\n");//printf("(P: %p, LC: %p, RC: %p)\n", match->parent, match->lChild, match->rChild);

	printNodeRecurse(match->lChild);
	printNodeRecurse(match->rChild);

}

void checkHeap(Matches * matches) {
	printf("Checking heap starting at root "); printMatch(matches->heap->root); printf("...\n");
	int numErr = 0;
	Match * m;
	bool conflict;
	bool left;
	bool right;
	for (int i = 0; i < matches->size; i++) {
		m = matches->start_arr[i];
		if (m) {
			conflict = false;
			left = true;
			right = true;;
			/* check with parent/children conflict */
			/* check its children */
			if (m->lChild && m->lChild->parent != m) conflict = true;
			if (m->rChild && m->rChild->parent != m) conflict = true;

			/* check its parent */
			if (m->parent && m->parent->lChild != m && m->parent->rChild != m) conflict = true;

			/* check with max in its branch conflict */
			bool maxPass = maxCheck(m);

			/* check that its child to the left is to the left and same with right. */
			if (m->lChild && middle(m->lChild) > middle(m)) left = false;
			if (m->rChild && middle(m->rChild) <= middle(m)) right = false;

			printf("(%i-%i,%i-%i [%i,%s] ... parents: %s, maxCheck: %s, lrCheck: %s <> %s)\n", m->start, m->end, m->cindex, m->cindex + m->end - m->start, matchLength(m), m->type == 1 ? "+" : "-", conflict ? "CONFLICTED" : "NO CONFLICT", maxPass ? "PASS" : "FAIL", left ? "PASS" : "FAIL", right ? "PASS" : "FAIL");
			if (conflict || !maxPass || !left || !right) numErr++;
		}
	}
	printf("\n\nHeap check completed with %i errors. %s\n\n", numErr, numErr == 0 ? "CONGRATS!!!" : "aw :*(");
}

bool maxCheck(Match * m) {
	int max = matchLength(m);

	bool l = m->lChild ? maxRecurse(m->lChild, max) : true;
	bool r = m->rChild ? maxRecurse(m->rChild, max) : true;
	return l && r;
}

bool maxRecurse(Match * m, int max) {
	if (matchLength(m) > max) return false;

	bool l = m->lChild ? maxRecurse(m->lChild, max) : true;
	bool r = m->rChild ? maxRecurse(m->rChild, max) : true;
	return l && r;
}

int printOptimumMatches(Matches * matches) {
	/* print out the root */
	printMatch(matches->heap->root); printf("\n");

	int a = matches->heap->root->start != 0 ? printOptimumRecurse(0, matches->heap->root->start, matches->heap->root->lChild, 1) : 1;
	int b = matches->heap->root->end != matches->size - 1 ? printOptimumRecurse(matches->heap->root->end, matches->size - 1, matches->heap->root->rChild, a) : a;

	return b;
}

int printOptimumRecurse(int lower, int upper, Match * root, int i) {
	/* print out the root */
	printMatch(root); printf("\n");
	i++;

	int a = root->start > lower ? printOptimumRecurse(lower, root->start, root->lChild, i) : i;
	int b = root->end < upper ? printOptimumRecurse(root->end, upper, root->rChild, a) : a;

	return b;
}