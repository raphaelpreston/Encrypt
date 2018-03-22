#pragma once
#include "stdafx.h"

/* structs */

typedef struct Match {
	int start;
	int end;
	int cindex;
	struct Match * next;
} match;


typedef struct Matches {
	match ** start_arr;
	match ** end_arr;
} matches;

/* returns a new match object */
match * newMatch(int body_begin, int crypt_begin, int length);

/* returns a new matches object */
matches * newMatches(int num_bits);
