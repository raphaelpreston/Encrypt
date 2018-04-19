#pragma once
#include "stdafx.h"

//a lot of lag is going to come from deleting not being optimized

/* structs */

typedef struct Match {
	int start;
	int end;
	int cindex;
	struct Match * start_next;	//next match in the start array
	struct Match * end_next;	//next match in the end array
	bool type;					//true for a positive match, false for a negative match
} Match;


typedef struct Matches {
	Match ** start_arr;	//array of match objects indexed by where the match starts
	Match ** end_arr;	//array of match objects indexed by where the match ends
	int size;			//size of each array, (designates upper and lower limits of matches), set to the number of bits that are read
	int num_matches;
	bool * used;		//1 if the bit is already matched, 0 if the bit is not
} Matches;

/* returns a new match object */
Match * newMatch(int body_begin, int crypt_begin, int length);

/* returns a new matches object */
Matches * newMatches(int num_bits);

/* returns length of match */
int matchLength(Match *);

/* adds the match to a matches */
void addMatch(Matches * matches, Match * m);

/* returns true if all bits in match m are already matched by other bits in the matches object (note:  it only checks if body is enveloped when adding, crypt being enveloped is fine) */
bool enveloped(Matches * matches, Match * m);

/* sets the bits in a matches object as used */
void set_bits_used(Matches * matches, Match * match);

/* prints a specific match */
void printMatch(Match * m);

/* prints matches */
void printMatches(Matches * m);

/* returns index of max matching within a range (for a specified pos/neg type) for either of the matches array */
Match * maxMatchInRange(Match ** arr, Match * m, int start, int end);

/* tests to see whether or not two matches are compatible: that is to say that the crypt parts of the matching can be merged */
bool cryptCompatable(Match * m1, Match * m2);

/* returns the length of what a merged matching between m1 and m2 would look like */
int * testMerge(Match * m1, Match * m2);

/* returns a match that is the merged match of all match objects in matches[]. Size is how many objects there are. Set any to NULL */
Match * merge(Match * matches[], int size);

/* deletes the exact match object given from the matches (both arrays).  Returns 1 on success 0 on failure. */
bool deleteMatch(Matches * matches, Match * m);		//doesn't actually free cus it doesn't work but it frees some stuff :DD