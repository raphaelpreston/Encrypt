#include "stdafx.h"
#include "Matchings.h"

match * newMatch(int b, int c, int l) {
	
	/* allocate space for match boye */
	match * m;
	m = (match *)malloc(sizeof(match));
	if (m) {
		
		/* fill boye */
		m->start = b;
		m->end = b + l - 1;
		m->cindex = c;
		m->next = NULL;

		return m;
	}
	else {
		printf("Failed to malloc space for a new match (b: %i, c: %i, l: %i)\n", b, c, l);
		return NULL;
	}
}

matches * newMatches(int num_bits) {
	
	/* allocate space */
	matches * m = (matches *)malloc(sizeof(matches));
	if (m) {

		/* make both arrays */
		match ** start;
		match ** end;
		start = (match **)calloc(num_bits, sizeof(match *));	//technically we can remove 1 because there are no endings at 0 
		end = (match **)calloc(num_bits, sizeof(match *));	// use calloc to initialize each match * to 0
		if (start && end) {
			
			/* fill m */
			m->start_arr = start;
			m->end_arr = end;
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


void addMatch(matches * matches , match * match) {
	
	/* add to arrays */
	matches->start_arr[match->start] = match;



}

//void deleteMatches(matches * m, int num_bits) {
//	for (int i = 0; i < num_bits; i++) {
//
//	}
//	free(m->begin_arr);
//	free(m->end_arr);
//}