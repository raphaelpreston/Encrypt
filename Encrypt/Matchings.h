#ifndef MATCHINGS_H
#define MATCHINGS_H

#include "Binary.h"
#include <stdbool.h>

//a lot of lag is going to come from deleting not being optimized
// see reAdd for more optimization
// see modifiedMatchLength and optimize it

/* structs */

typedef struct Match {
	int start;
	int end;
	int cindex;
	struct Match * lChild;		//for the MatchHeap
	struct Match * rChild;
	struct Match * parent;
	int type;					//1 for a positive match, 2 for a negative match
} Match;


typedef struct Matches {
	struct Match ** start_arr;	//array of match objects indexed by where the match starts
	struct Match ** end_arr;	//array of match objects indexed by where the match ends
	struct MatchHeap * heap;	//heap used to store the max length matches and map where they are relative to eachother
	int size;			//size of each array, (designates upper and lower limits of matches), set to the number of bits that are read
	int bits_covered;	//an integer that keeps track of the biggest ending number of a match (used in optimum functions)
	int num_matches;
	bool * used;		//1 if the bit is already matched, 0 if the bit is not
} Matches;


//code based of off Robin Thomas's GitHub version
/* properties of this heap:
1. A parent match will always have a bigger length
2. A parent's left child's middle is to the left or equal to itself
3. A parent's right child's middle is to the right of itself
*/

typedef struct MatchHeap {
	int size;
	struct Match * root;
} MatchHeap;



/* returns a new match object */
Match * newMatch(int body_begin, int crypt_begin, int length, int type);

/* returns a new matches object */
Matches * newMatches(int num_bits);

/* returns length of match */
int matchLength(Match *);

/* adds the match to a matches */
// void addMatch(Binary * binary, Matches * matches, Match * m);

/* prints a specific match */
void printMatch(Match * m);

/* returns if a match has valid memory matches */
//void isValid(Match * m, struct Binary * binary);

/* test whether a set of matches is valid */
//void printMatchesValidity(Matches * m, struct Binary * b);

/* prints matches*/
 void printMatches(Matches * m);

/* returns index of max matching within a range (for a specified pos/neg type) for either of the matches array */
Match * maxMatchInRange(Match ** arr, Match * m, int start, int end);

/* tests to see whether or not two matches are compatible: that is to say that the crypt parts of the matching can be merged (correct number and correct side)*/
bool cryptCompatable(Match * m1, Match * m2);

/* returns the length of what a merged matching between m1 and m2 would look like */
int testMerge(Match * m1, Match * m2);

/* returns a match that is the merged match of all match objects in matches[]. Size is how many objects there are. Set any to NULL */
Match * merge(Match * matches[], int size);

/* deletes the exact match object given from the matches (both arrays).  Returns 1 on success 0 on failure. */
bool deleteMatch(Matches * matches, Match * m);		//doesn't actually free cus it doesn't work but it frees some stuff :DD

/* returns middle of match, -1 to favor left, 1 to favor right */
double middle(Match * match);

/* swaps all values of both matches */
void swapValues(Match * a, Match * b);

/* assigns the values of a to b */
void assignVals(Match * a, Match * b);

/* returns the match length modified by start and end limits */
int modifiedMatchLength(Match * m, int start, int end);

/* MATCHHEAP */


MatchHeap * newMatchHeap();

/* percolate down */
void heap_insertMatch(Matches * matches, Match * match);

/* helper function for insertNode */
void heap_insertRecurse(MatchHeap * heap, Match * root, Match * match);

/* recursively print out the heap (in a very cryptic annoying way) */
void printHeap(MatchHeap * heap);

void printNodeRecurse(Match * match);

void checkHeap(Matches * matches);

bool maxCheck(Match * match);

bool maxRecurse(Match * m, int max);

/* basically inserts match as the new root of a minitree.  Returns true if the root and the mini tree went to the left of the match, false otherwise */
bool rootReplace(Match * root, Match * match);

/* returns true if child belongs to the left of parent, false otherwise	*/
bool goesToLeft(Match * child, Match * parent);

/* reAdds all branches starting with root to the destination match (don't readd at a heap's root cus the heap won't have it's root pointer updated 
   Note: could make this faster by not adding every single thing below the node, just the ones that have to be readded */
void reAdd(Match * root, Match * dest);

/* prints the minimum amount of matches needed to cover the entire body, returns the number of matches needed */
int printOptimumMatches(Matches * matches);

int printOptimumRecurse(int lower, int upper, Match * root, int i);

/* returns the number of digits in the number */
int numDecDigits(unsigned int num);

int numBinDigits(unsigned int num);
#endif
