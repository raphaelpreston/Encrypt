#pragma once
#include "stdafx.h"

/* structs */

typedef struct Match {
	int start;
	int end;
	int cindex;
	struct Match * start_next;	//next match in the start array
	struct Match * end_next;	//next match in the end array
} Match;


typedef struct Matches {
	Match ** start_arr;
	Match ** end_arr;
	int size;
} Matches;

/* returns a new match object */
Match * newMatch(int body_begin, int crypt_begin, int length);

/* returns a new matches object */
Matches * newMatches(int num_bits);

/* adds the match to a matches */
void addMatch(Matches * matches, Match * m);

/* prints matches */
void printMatches(Matches * m);

/* prints a specific match */
void printMatch(Match * m);

/* returns length of match */
int matchLength(Match *);

/* returns max matching within a range for either of the matches array */
Match * maxMatchInRange(Match ** arr, Match * m, int start, int end);

/* returns the length of what a merged matching between m1 and m2 would look like */
int * testMerge(Match * m1, Match * m2);

/* returns a match that is the merged match of a, b, c.  Set any to NULL */
Match * merge(Match * a, Match * b, Match * c);

/* tests to see whether or not two matches are compatible: that is to say that the crypt parts of the matching can be merged */
bool cryptCompatable(Match * m1, Match * m2);