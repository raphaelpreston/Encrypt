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


void addMatch(matches * matches , match * m) {
	int length = m->end - m->start;

	/* add to arrays */
	int start = m->start;
	if (matches->start_arr[start] == 0) {	//first match * in this linked list
		matches->start_arr[start] = m;
	}
	else {
		
		/* keep max length in front */
		match * curr = matches->start_arr[start];
		while (curr->next != NULL && (curr->next->end - curr->next->start) > length) {	//keep looking until curr->next is null or curr->next is shorter or equal to in length than m
			match * next = curr->next;
			curr->next = m;
			m->next = next;	//will point to null, should be ok
		}
	}
}


//void deleteMatches(matches * m, int num_bits) {
//	for (int i = 0; i < num_bits; i++) {
//
//	}
//	free(m->begin_arr);
//	free(m->end_arr);
//}