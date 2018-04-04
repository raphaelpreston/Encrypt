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

		/* make the array to keep track of matched bits */
		bool * used = (bool *)calloc(num_bits, sizeof(bool));

		if (start && end) {

			/* fill m */
			m->start_arr = start;
			m->end_arr = end;
			m->size = num_bits;
			m->num_matches = 0;
			m->used = used;
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
	printf("Attempting to add match: "); printMatch(m); printf("\n");
	int length = matchLength(m);
	Match * temp;
	Match * curr;

	/* error check for exceeding limit of arrays */
	if (m->end >= matches->size || m->start < 0) {
		printf("\nMatch "); printMatch(m); printf(" out of bounds of matches arrays.\n");
	}

	/* make sure all bits in the match aren't already used up */
	if (!enveloped(matches, m)) {
		/* set the bits as used */
		set_bits_used(matches, m);

		/* get the max compatable match from all max matches that start in range m_start to m_end + 1 and all that end in start - 1 to end (that match in both crypt and body)*/

		if (!matches->num_matches == 0) {	//only look for max matches if the arrays are not empty
			printf("\nStart_arr: ");
			Match * max_start = maxMatchInRange(matches->start_arr, m, m->start, m->end + 1 >= matches->size ? m->end : m->end + 1);	//make sure the upper limit is in bounds
			printf("\nEnd arr:");
			Match * max_end = maxMatchInRange(matches->end_arr, m, m->start - 1 < 0 ? m->start : m->start - 1, m->end);		//make sure the lower limit is in bounds

			if (max_start != NULL) {
				printf("The max start matching found in range [%i,%i] was ", m->start, m->end + 1 >= matches->size ? m->end : m->end + 1); printMatch(max_start); printf("\n");
			}
			else printf("No max start match found in range [%i,%i]", m->start, m->end + 1 >= matches->size ? m->end : m->end + 1); printf("\n");

			if (max_end != NULL) {
				printf("The max end matching found in range [%i,%i] was ", m->start - 1 < 0 ? m->start : m->start - 1, m->end); printMatch(max_end); printf("\n");
			}
			else printf("No max end match found in range [%i,%i]", m->start - 1 < 0 ? m->start : m->start - 1, m->end); printf("\n");

			// merge the 0/2/3 to get a supermatch
			if (max_start || max_end) {	//merge() can handle if one is NULL but no reason to call if they both are
				Match * max_matches[] = { m, max_start, max_end };
				Match * merged = merge(max_matches, 3);
				if (merged != NULL) {
					printf("New merged baby match: "); printMatch(merged); printf(" \n");

					// delete the 1/2 out of both arrays
					if (max_start) { //delete it because it got merged
						printf("Attempting to delete the max_start: "); printMatch(max_start); printf("\n");
						deleteMatch(matches, max_start);

					}
					if (max_end) {
						printf("Attempting to delete the max_end: "); printMatch(max_end); printf("\n");
						deleteMatch(matches, max_end);
					}

					// now we are adding the supermatch, so m = supermatch
					m = merged;
					printf("Now adding merged match: "); printMatch(m); printf("\n");
				}
				else {
					printf("The merge was unsuccsesful because they came up with different body/crypt lengths.\n");
				}

			}
			else printf("No attempt to merge because max start and max end were both NULL.\n");


			//printf("Attempting to merge:\n");
		}
		else {
			printf("Matches were empty so no merging was attempted.\n");
		}



		/* add to start_arr */
		int start = m->start;
		if (matches->start_arr[start] == NULL) {	//first match * in this linked list
			matches->start_arr[start] = m;
		}
		else {

			/* keep max length in front */
			curr = matches->start_arr[start];
			if ((curr->end - curr->start + 1) < length) {	//if the match to add is bigger than the first one, make it the head 
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
		matches->num_matches++;			//added one match to the matches
	}
	else {	//is enveloped
		printf("Match was enveloped, didn't do anything. \n");
	}
}

bool enveloped(Matches * matches, Match * match) {
	int l = match->start + matchLength(match);	//don't subtract 1 here and use < instead
	for (int i = match->start; i < l; i++) {
		if (!matches->used[i]) return false;
	}
	return true;
}

void set_bits_used(Matches * matches, Match * match) {
	int l = match->start + matchLength(match);	//don't subtract 1 here and use < instead
	for (int i = match->start; i < l; i++) {
		matches->used[i] = true;
	}
}

void printMatch(Match * m) {
	printf("(%i-%i,%i-%i [%i] - %p)", m->start, m->end, m->cindex, m->cindex + m->end - m->start, m->end - m->start + 1, m);
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
	printf("Num matches: %i\n", m->num_matches);
}

Match * maxMatchInRange(Match ** arr, Match * m, int start, int end) {
	Match * max;
	Match * curr;
	max = arr[m->start] == NULL || !cryptCompatable(m, arr[m->start]) ? NULL : arr[m->start];	//we are trying to find the max compatable pair to merge (must test to make sure it's not NULL first
	printf("\n");
	for (int i = start; i <= end; i++) {		//must retest for some reason i dunno why lol
		curr = arr[i];	//current max match
		if (curr != NULL && cryptCompatable(m, curr)) {	//have to test to make sure it's not null otherwise cryptComp will throw an error
			if (curr != NULL) {
				// printf("Comparing current: "); printMatch(curr); printf(" against max: "); if (max == NULL)printf("NULL"); else printMatch(max); printf("\n");
				if (!max == NULL) printf("Is %i > %i?\n", curr->end - m->start, max->end - m->start);
				if (max == NULL) {

					if (max == NULL || testMerge(curr, m) > testMerge(max, m)) {
						max = curr;
						// printf("Yes, assigned.\n");
					}
				}
				// else printf("No, skipped.\n");
			}
			else {
				// printf("Didn't execute because curr is %s and max_start is %s\n", curr == NULL ? "NULL" : "NOT NULL", max == NULL ? "NULL" : "NOT NULL");
			}
		}
		// else printf("The two matches weren't compatable or one was NULL.\n");
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
	return true;
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

Match * merge(Match * matches[], int size) {	//will return NULL if merge made unequal body/crypt match.
												// IF THEY AREN'T COMPATABLE U MESSED UP
	Match * match;
	int min_b_start = matches[0]->start;
	int max_b_end = matches[0]->end;
	int min_c_start = matches[0]->cindex;
	int max_c_end = matches[0]->cindex + matchLength(matches[0]) - 1;	//matches[0] can't be NULL

	for (int i = 1; i < size; i++) {	//take the least start and the greatest end
		if (matches[i] != NULL) {
			// min/max for body
			if (matches[i]->start < min_b_start) min_b_start = matches[i]->start;
			if (matches[i]->end > max_b_end) max_b_end = matches[i]->end;

			//min/max for crypt
			if (matches[i]->cindex < min_c_start) min_c_start = matches[i]->cindex;
			if (matches[i]->cindex + matchLength(matches[i]) - 1 > max_c_end) max_c_end = matches[i]->cindex + matchLength(matches[i]) - 1;
		}
	}

	/* do the real merging */
	// printf("\n(%i, %i, %i)", matches[1]->start, matches[1]->cindex, matches[1]->end - matches[1]->start + 1);
	// printf("\nmin_b_start: %i\nmax_b_end: %i\nmin_c_start: %i\nmax_c_end:%i",  min_b_start, max_b_end, min_c_start, max_c_end);
	if (max_b_end - min_b_start != max_c_end - min_c_start) return NULL;	//when all merged together, the length of the body match must be equal to the length of the crypt match

	match = newMatch(min_b_start, min_c_start, max_b_end - min_b_start + 1);
	return match;
}

bool deleteMatch(Matches * matches, Match * match) {
	if (matches->size == 0) return false;
	Match * curr;
	Match * temp;

	//delete from start_arr by finding index of start
	curr = matches->start_arr[match->start];
	if (curr == match) {	//simply reassign head
		temp = matches->start_arr[match->start];
		matches->start_arr[match->start] = matches->start_arr[match->start]->start_next;
	}
	else {
		while (curr->start_next != NULL && curr->start_next != match) {	//ends when curr->next is NULL or curr == match to delete
			curr = curr->start_next;
		}
		// delete it
		if (curr->start_next != NULL) {
			temp = curr->start_next;
			curr->start_next = temp->start_next;
			return true;
		}
		else return false;		//looped all the way through without reaching it
	}

	//repeat with end_arr
	curr = matches->end_arr[match->end];
	if (curr == match) {	//simply reassign head
		temp = matches->end_arr[match->end];
		matches->end_arr[match->end] = matches->end_arr[match->end]->end_next;
		free(temp);
	}
	else {
		while (curr->end_next != NULL && curr->end_next != match) {	//ends when curr->next is NULL or curr == match to delete
			curr = curr->end_next;
		}
		// delete it
		if (curr->end_next != NULL) {
			temp = curr->end_next;
			curr->end_next = temp->end_next;
			free(temp);
			return true;
		}
		else return false;		//looped all the way through without reaching it
	}
	// free(match); doesn't work lol
	matches->num_matches--;
}

//void deleteMatches(matches * m, int num_bits) {
//	for (int i = 0; i < num_bits; i++) {
//
//	}
//	free(m->begin_arr);
//	free(m->end_arr);
//}



