#include "stdafx.h"
#include "Matchings.h"

Match * newMatch(int b, int c, int l) {
	
	/* allocate space for match boye */
	Match * m;
	m = (Match *)malloc(sizeof(Match));
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

Matches * newMatches(int num_bits) {
	
	/* allocate space */
	Matches * m = (Matches *)malloc(sizeof(Matches));
	if (m) {

		/* make both arrays */
		Match ** start;
		Match ** end;
		start = (Match **)calloc(num_bits, sizeof(Match *));	//technically we can remove 1 because there are no endings at 0 
		end = (Match **)calloc(num_bits, sizeof(Match *));	// use calloc to initialize each match * to 0
		if (start && end) {
			
			/* fill m */
			m->start_arr = start;
			m->end_arr = end;
			m->size = num_bits;
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


void addMatch(Matches * matches , Match * m) {
	int length = m->end - m->start + 1;
	Match * curr;
	
	/* error check for exceeding limit of arrays */
	if (m->end >= matches->size || m->start < 0) {
		printf("\nMatch "); printMatch(m); printf(" out of bounds of matches arrays.\n");
	}
	if (length < 2) {
		printf("\nWarning: length of match "); printMatch(m); printf(" is %i\n", length);
	}

	/* add to start_arr */
	int start = m->start;
	if (matches->start_arr[start] == 0) {	//first match * in this linked list
		matches->start_arr[start] = m;
	}
	else {
		
		/* keep max length in front */
		curr = matches->start_arr[start];
		while (curr->next != NULL && (curr->next->end - curr->next->start) > length) {	//keep looking until curr->next is null or curr->next is shorter or equal to in length than m
			Match * next = curr->next;
			curr->next = m;
			m->next = next;	//will point to null, should be ok
		}
	}

	/* repeat with end_arr*/
	int end = m->end;
	if (matches->end_arr[end] == 0) {	//first match * in this linked list
		matches->end_arr[end] = m;
	}
	else {

		/* keep max length in front */
		curr = matches->end_arr[end];
		while (curr->next != NULL && (curr->next->end - curr->next->start) > length) {	//keep looking until curr->next is null or curr->next is shorter or equal to in length than m
			Match * next = curr->next;
			curr->next = m;
			m->next = next;	//will point to null, should be ok
		}
	}
}

void printMatch(Match * m) {
	printf("(%i<>%i,%i<>%i)", m->start, m->end, m->cindex, m->cindex + m->end - m->start);
}

void printMatches(Matches * m) {
	Match ** start = m->start_arr;
	Match ** end = m->end_arr;
	Match * curr;

	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (start[i] != NULL) {
			do {
				printf(" "); printMatch(start[i]); printf("\n");
			} while (start[i]->next != NULL);
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