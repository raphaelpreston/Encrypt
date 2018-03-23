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