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
		m->start_next = NULL;
		m->end_next = NULL;

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

int matchLength(Match * m) {
	return m->end - m->start + 1;
}

void addMatch(Matches * matches , Match * m) {
	int length = matchLength(m);
	Match * temp;
	Match * curr;
	
	/* error check for exceeding limit of arrays */
	if (m->end >= matches->size || m->start < 0) {
		printf("\nMatch "); printMatch(m); printf(" out of bounds of matches arrays.\n");
	}
	
	/* get the max compatable match from all max matches that start in range m_start to m_end + 1 and all that end in start - 1 to end (that match in both crypt and body)*/
	printf("\nStart_arr: ");
	Match * max_start = maxMatchInRange(matches->start_arr, m, m->start, m->end + 1);
	printf("\nEnd arr:");
	Match * max_end = maxMatchInRange(matches->end_arr, m, m->start - 1, m->end);

	if (max_start != NULL) {
		printf("The max start matching found in range [%i,%i] was ", m->start, m->end + 1); printMatch(max_start); printf("\n");
	}
	else printf("No max start match found in range [%i,%i]", m->start, m->end + 1); printf("\n");

	if (max_end != NULL) {
		printf("The max end matching found in range [%i,%i] was ", m->start - 1, m->end); printMatch(max_end); printf("\n");
	}
	else printf("No max end match found in range [%i,%i]", m->start - 1, m->end); printf("\n");


	// merge the 0/2/3 to get a supermatch
	// delete the 1/2 out of both arrays
	// now we are adding the supermatch, so m = supermatch


	/* add to start_arr */
	int start = m->start;
	if (matches->start_arr[start] == NULL) {	//first match * in this linked list
		matches->start_arr[start] = m;
	}
	else {
		
		/* keep max length in front */
		curr = matches->start_arr[start];
		if((curr->end - curr->start + 1) < length){	//if the match to add is bigger than the first one, make it the head 
			m->start_next = curr;
			matches->start_arr[start] = m;
		}
		else {	//loop through until we find appropriate spot
			while (curr->start_next != NULL && (curr->start_next->end - curr->start_next->start + 1) > length) {	//keep looking until curr->next is null or curr->next is shorter or equal to in length than m
				curr = curr->start_next;
			}
			temp = curr->start_next;	//either null or <=
			curr->start_next = m;
			m->start_next = temp;	//will point to null, should be ok
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
		if ((curr->end - curr->start + 1) < length) {	//if the match to add is bigger than the first one, make it the head 
			m->end_next = curr;
			matches->end_arr[end] = m;
		}
		else {	//loop through until we find appropriate spot
			while (curr->end_next != NULL && (curr->end_next->end - curr->end_next->start + 1) > length) {	//keep looking until curr->next is null or curr->next is shorter or equal to in length than m
				curr = curr->end_next;
			}
			temp = curr->end_next;	//either null or <=
			curr->end_next = m;
			m->end_next = temp;	//will point to null, should be ok
		}
	}
}

void printMatch(Match * m) {
	printf("(%i-%i,%i-%i [%i])", m->start, m->end, m->cindex, m->cindex + m->end - m->start, m->end - m->start + 1);
}

void printMatches(Matches * m) {
	Match ** start = m->start_arr;
	Match ** end = m->end_arr;
	Match * curr;

	printf("Start Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (start[i] != NULL) {
			printf(" "); printMatch(start[i]); printf("\n");
			curr = start[i];
			while (curr->start_next != NULL) {
				printf(" "); printMatch(curr->start_next); printf("\n");
				curr = curr->start_next;
			}
		}
	}

	printf("End Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (end[i] != NULL) {
			printf(" "); printMatch(end[i]); printf("\n");
			curr = end[i];
			while (curr->end_next != NULL) {
				printf(" "); printMatch(curr->end_next); printf("\n");
				curr = curr->end_next;
			}
		}
	}
}

Match * maxMatchInRange(Match ** arr, Match * m, int start, int end) {
	Match * max;
	Match * curr;
	max = compatable(m, arr[m->start])?arr[m->start]:NULL;	//we are trying to find the max compatable pair to merge
	printf("\n");
	for (int i = start + 1; i <= end; i++) {
		curr = arr[i];	//current max match
		if (compatable(m, curr)) {
			if (curr != NULL) {
				printf("Comparing current: "); printMatch(curr); printf(" against max: "); if (max == NULL)printf("NULL"); else printMatch(max); printf("\n");
				if (!max == NULL) printf("Is %i > %i?\n", curr->end - m->start, max->end - m->start);
				if (max == NULL) {

					if (max == NULL || (testMerge(curr, m) > testMerge(max, m))) {
						max = curr;
						printf("Yes, assigned.\n");
					}
				}
				else printf("No, skipped.\n");
			}
			else {
				printf("Didn't execute because curr is %s and max_start is %s\n", curr == NULL ? "NULL" : "NOT NULL", max == NULL ? "NULL" : "NOT NULL");
			}
		}
	}
	return max;
}

int * testMerge(Match * a, Match * b) {
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

Match * merge(Match ** matches, int size) {
	Match * newMatch;
	Match * min_start = matches[0];
	Match * max_end = matches[0];

	for (int i = 0; i < size; i++) {	//take the least start and the greatest end
		if (matches[i] != NULL) {
			if(matches[i]->start < min_start)
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