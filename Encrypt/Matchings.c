#include <stdio.h>
#include <stdint.h>
 
#include "Matchings.h"
#include "Binary.h"


#define PRINT 0
#define UPLE_CONSTANT 2		//check this method for efficieny improvement

Match * newMatch(int b, int c, int l, int type) {
	/* allocate space for match boye */
	Match * m;
	m = (Match *)malloc(sizeof(Match));
	if (m) {

		/* fill boye */
		m->start = b;
		m->end = b + l - 1;
		m->cindex = c;
		m->type = type;

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
		start = (Match **)calloc(num_bits, sizeof(Match *));	//technically we can remove 1 because there are no endings at 0 <-- not correct i think
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

			m->max_body_start = 0;		//headers for map file
			m->max_crypt_start = 0;
			m->max_length = 0;
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

bool isValid(Match * m, Binary * b) {
	bool valid = true;
	int fail;

	int bi;
	int ci;

	for (int i = 0; i + m->start <= m->end; i++) {	//test each reference point
		bi = b->body->arr[m->start + i];
		ci = b->crypt->arr[m->cindex + i];

		if ((bi != ci && m->type == 1) || (bi == ci && m->type == 2)) {
			if (valid) fail = m->start + i;	//assign it on first fail but not again
			valid = false;
		}
	}

	//if (!valid) printf("(%i-%i,%i-%i [%i,%s] ... %s)", m->start, m->end, m->cindex, m->cindex + m->end - m->start, matchLength(m), m->type == 1 ? "+" : "-", valid ? "VALID" : "NOT VALID");
	return valid;
}

void addMatch(Binary * binary, Matches * matches, Match * m) {
	if (!isValid(m, binary)) {printf("Attempting to add invalid match: "); printMatch(m); printf("\n");}
	if (PRINT == 1) { printf("Attempting to add match: "); printMatch(m); printf("\n"); }
	int length = matchLength(m);
	Match * curr;

	/* error check for exceeding limit of arrays */
	if (m->start < 0) printf("\n\nERROR: Start of match was less than 0...\n\n\n\n");

	if (m->end >= matches->size) {	//reallocate mems
		int newSize = matches->size;

		while (newSize < m->end) newSize *= UPLE_CONSTANT;
		//newSize is new number of bits to hold

		Match ** newStart;
		Match ** newEnd;
		newStart = (Match **)calloc(newSize, sizeof(Match *));
		newEnd = (Match **)calloc(newSize, sizeof(Match *));	// use calloc to initialize each match * to 0

		if (!newStart || !newEnd) printf("Error mallocing %i bit spots in the array for matches.\n", newSize);

		//fill it with old content
		if (matches->size != 0) {
			memcpy(newStart, matches->start_arr, matches->size * sizeof(Match *));
			memcpy(newEnd, matches->end_arr, matches->size * sizeof(Match *));
		}


		//free old memory, reasign buffer
		free(matches->start_arr);
		free(matches->end_arr);

		matches->start_arr = newStart;
		matches->end_arr = newEnd;

		//update size
		matches->size = newSize;
	}


	/* get the max compatable match from all max matches that start in range m_start to m_end + 1 and all that end in start - 1 to end (that match in both crypt and body)*/

	if (!matches->num_matches == 0) {	//only look for max matches if the arrays are not empty
		Match * max_start = maxMatchInRange(matches->start_arr, m, m->start, m->end + 1 >= matches->size ? m->end : m->end + 1);	//make sure the upper limit is in bounds
		Match * max_end = maxMatchInRange(matches->end_arr, m, m->start - 1 < 0 ? m->start : m->start - 1, m->end);		//make sure the lower limit is in bounds

		if (max_start != NULL) {
			if (PRINT == 1) { printf("The max start matching found in range [%i,%i] was ", m->start, m->end + 1 >= matches->size ? m->end : m->end + 1); printMatch(max_start); printf("\n"); }
		}
		else {
			if (PRINT == 1){ printf("No max start match found in range [%i,%i]", m->start, m->end + 1 >= matches->size ? m->end : m->end + 1); printf("\n");}
		}

		if (max_end != NULL) {
			if (PRINT == 1) { printf("The max end matching found in range [%i,%i] was ", m->start - 1 < 0 ? m->start : m->start - 1, m->end); printMatch(max_end); printf("\n"); }
		}
		else {
			if (PRINT == 1) { printf("No max end match found in range [%i,%i]", m->start - 1 < 0 ? m->start : m->start - 1, m->end); printf("\n"); }
		}

		// merge the 0/2/3 to get a supermatch
		if (max_start || max_end) {	//merge() can handle if one is NULL but no reason to call if they both are
			Match * max_matches[] = { m, max_start, max_end };
			Match * merged = merge(max_matches, 3);
			if (merged != NULL) {
				if (PRINT == 1) { printf("New merged baby match: "); printMatch(merged); printf(" \n"); }

				// delete the 1/2 out of both arrays
				if (max_start == max_end) {	//protect against double delete
					if (max_start) {
						if (PRINT == 1) { printf("Attempting to delete the max_start: "); printMatch(max_start); printf("\n"); }
						deleteMatch(matches, max_start);
					}
				}
				else {
					if (max_start) {
						if (PRINT == 1) {printf("Attempting to delete the max_start: "); printMatch(max_start); printf("\n");}
						deleteMatch(matches, max_start);
					}
					if (max_end) {
						if (PRINT == 1) { printf("Attempting to delete the max_end: "); printMatch(max_end); printf("\n"); }
						deleteMatch(matches, max_end);
					}
				}

				// now we are adding the supermatch, so m = supermatch
				m = merged;
				if (PRINT == 1) { printf("Now adding merged match: "); printMatch(m); printf("\n"); }
			}
			else {
				if (PRINT == 1) printf("The merge was unsuccsesful because they came up with different body/crypt lengths.\n");
			}

		}
		else {
			if (PRINT == 1) printf("No attempt to merge because max start and max end were both NULL.\n");
		}


		if (PRINT == 1) printf("Attempting to merge:\n");
	}
	else {
		if (PRINT == 1) printf("Matches were empty so no merging was attempted.\n");
	}

	if (PRINT == 1) { printf("Adding match: "); printMatch(m); printf("\n"); }
	length = matchLength(m); //update length

	/* add to start_arr */
	int start = m->start;
	// bool used = false;
	if (matches->start_arr[start] == NULL) {	//first match * in this linked list
		matches->start_arr[start] = m;
	}
	else {

		/* keep max length in front */
		curr = matches->start_arr[start];	//the max
		if ((curr->end - curr->start + 1) < length) {	//if the match to add is bigger than the first one, change the values of the current one to match the new one
			//assignVals(m, matches->start_arr[start]);
			matches->start_arr[start] = m;
			// used = true;
		}
	}

	/* repeat with end_arr*/ // should be 
	int end = m->end;
	if (matches->end_arr[end] == NULL) {	//first match * in this linked list
		matches->end_arr[end] = m;
	}
	else {

		/* keep max length in front */
		curr = matches->end_arr[end];
		if ((curr->end - curr->start + 1) < length) {	//if the match to add is bigger than the first one, replace it and free old one
			// free(curr);
			matches->end_arr[end] = m;
			// assignVals(m, matches->end_arr[end]);
		}
		else {
			//if (!used) free(m);	//wasn't used in start array so we can trash it
		}
	}

	matches->num_matches++;			//added one match to the matches
	if(m->end > matches->bits_covered) matches->bits_covered = m->end;	//update bits_covered to the end of the match if it's bigger

	if (PRINT == 1) printf("Match adding process was completed (attempted).\n");
}

void printMatch(Match * m){
	if(m) printf("(%i-%i,%i-%i [%i,%s] - %.1f - %p)", m->start, m->end, m->cindex, m->cindex + m->end - m->start, matchLength(m), m->type == 1 ? "+" : "-", middle(m), m);
}

void printMatchesValidity(Matches * m, Binary * b) {
	Match ** start = m->start_arr;
	Match ** end = m->end_arr;
	int fails = 0;
	printf("Checking validity of matches...\n");
	printf("Start Array:\n");
	for (int i = 0; i < m->bits_covered + 1; i++) {
		if (start[i] != NULL) {
			if (!isValid(start[i], b)) {
				printf("%i\n", i);
				printMatch(start[i]);
				printf(" is NOT VALID\n");
				fails++;
			}
		}
	}
	printf("%i fails dissovered in %i tests.\n", fails, m->bits_covered + 1);
	fails = 0;
	printf("End Array:\n");
	for (int i = 0; i < m->bits_covered + 1; i++) {
		if (end[i] != NULL) {
			if (!isValid(end[i], b)) {
				printf("%i\n", i);
				printMatch(end[i]);
				printf(" is NOT VALID\n");
				fails++;
			}
		}
	}
	printf("%i fails dissovered in %i tests.\n", fails, m->bits_covered + 1);
	printf("Num matches: %i\n", m->num_matches);
}

void printMatches(Matches * m) {
	Match ** start = m->start_arr;
	Match ** end = m->end_arr;

	printf("Start Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (start[i] != NULL) {
			printf(" "); printMatch(start[i]); printf("\n");
		}
	}

	printf("End Array:\n");
	for (int i = 0; i < m->size; i++) {
		printf("%i\n", i);
		if (end[i] != NULL) {
			printf(" "); printMatch(end[i]); printf("\n");
		}
	}
	printf("Num matches: %i\n", m->num_matches);
	printf("Size of matches: %i\n", m->size);
	printf("Bits covered: %i\n", m->bits_covered);
}

Match * maxMatchInRange(Match ** arr, Match * m, int start, int end) {
	Match * max;
	Match * curr;
	max = arr[m->start] == NULL || arr[m->start] != m->type || !cryptCompatable(m, arr[m->start]) ? NULL : arr[m->start];	//we are trying to find the max compatable pair to merge (must test to make sure it's not NULL first and that its the same type
	if (PRINT == 1)  printf("\n");
	for (int i = start; i <= end; i++) {		//must retest for some reason i dunno why lol
		curr = arr[i];	//current max match
		if (curr != NULL && m->type == curr->type && cryptCompatable(m, curr)) {	//have to test to make sure it's not null otherwise cryptComp will throw an error (also make sure it's crypt comp and same type)
			if (curr != NULL) {
				if (PRINT == 1) { printf("Comparing current: "); printMatch(curr); printf(" against max: "); if (max == NULL)printf("NULL"); else printMatch(max); printf("\n"); }
				if (PRINT == 1) { if (!max == NULL) printf("Is %i > %i?\n", curr->end - m->start, max->end - m->start); }
				if (max == NULL) {

					if (max == NULL || testMerge(curr, m) > testMerge(max, m)) {
						max = curr;
						if (PRINT == 1) printf("Yes, assigned.\n");
					}
				}
				else if(PRINT == 1) printf("No, skipped.\n");
			}
			else {
				if (PRINT == 1) printf("Didn't execute because curr is %s and max_start is %s\n", curr == NULL ? "NULL" : "NOT NULL", max == NULL ? "NULL" : "NOT NULL");
			}
		}
		else {
			if (PRINT == 1) printf("The two matches weren't compatable or one was NULL.\n");
		}
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
	
	/* also return false if the sides aren't aligned */
	if (!(m2->start >= m1->start && m2->cindex >= m1->cindex) || !(m2->start <= m1->start && m2->cindex <= m1->cindex)) return false;

	return true;
}

int testMerge(Match * a, Match * b) {
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
			//type check
			if (matches[i]->type != matches[0]->type)printf("WARNING: MERGING MATCHES OF DIFFERENT TYPES!!\n\n");
			// min/max for body
			if (matches[i]->start < min_b_start) min_b_start = matches[i]->start;
			if (matches[i]->end > max_b_end) max_b_end = matches[i]->end;

			//min/max for crypt
			if (matches[i]->cindex < min_c_start) min_c_start = matches[i]->cindex;
			if (matches[i]->cindex + matchLength(matches[i]) - 1 > max_c_end) max_c_end = matches[i]->cindex + matchLength(matches[i]) - 1;
		}
	}

	/* do the real merging */
	if (PRINT == 1) printf("\n(%i, %i, %i)", matches[1]->start, matches[1]->cindex, matches[1]->end - matches[1]->start + 1);
	if (PRINT == 1) printf("\nmin_b_start: %i\nmax_b_end: %i\nmin_c_start: %i\nmax_c_end:%i",  min_b_start, max_b_end, min_c_start, max_c_end);
	if (max_b_end - min_b_start != max_c_end - min_c_start) return NULL;	//when all merged together, the length of the body match must be equal to the length of the crypt match

	match = newMatch(min_b_start, min_c_start, max_b_end - min_b_start + 1, matches[0]->type);
	return match;
}

bool deleteMatch(Matches * matches, Match * match) {
	if (matches->size == 0) return false;
	Match * curr;
	Match * temp;

	//delete from start_arr by finding index of start
	curr = matches->start_arr[match->start];
	if (curr == match) {	//simply remove head
		temp = matches->start_arr[match->start];
	}
	else {
		return false; //this shouldn't happen
	}

	//repeat with end_arr
	curr = matches->end_arr[match->end];
	if (curr == match) {	//simply reassign head
		free(matches->end_arr[match->end]);
	}
	else {
		// delete it
		return false;		//this shouldn't happen
	}
	// free(match); doesn't work lol
	matches->num_matches--;
	return true;
}

int modifiedMatchLength(Match * m, int start, int end) {	//modified match length
	//if (m->end <= start || m->start >= end) return matchLength(m); //completely outside limits
	//if (m->start >= start && m->end <= end) return 0;	//enveloped or exactly same length
	//if (m->start >= start && m->start < end && m->end > end) return matchLength(m) - (end - m->start);	//starts inside and ends to the right
	//if (m->start < start && m->end <= end && m->end > start) return matchLength(m) - (m->end - start);	//starts to the left and ends inside
	//else printf("YOU DIDN'T ACCOUNT FOR SOMETHING LMAO\n\n\n\n\n");
	int length = 0;
	for (int i = 0; i < matchLength(m); i++) {
		if (!(m->start + i <= end && m->start + i >= start)) length++;
	}
	return length;
}

double middle(Match * m) {
	double length = (double)matchLength(m);
	return m->start + length / 2.0;
}

void swapValues(Match * a, Match * b) {
	Match temp = *a;

	// assign all b stuff to a
	a->start = b->start;
	a->end = b->end;
	a->cindex = b->cindex;
	a->type = b->type;

	//assign all a stuff to b
	b->start = temp.start;
	b->end = temp.end;
	b->cindex = temp.cindex;
	b->type = temp.type;

	//printf("Post swap:         "); printMatch(a); printf(" and "); printMatch(b); printf("\n");
}

void assignVals(Match * a, Match * b) {
	Match temp = *a;

	//assign all a stuff to b
	b->start = temp.start;
	b->end = temp.end;
	b->cindex = temp.cindex;
	b->type = temp.type;
	//b->lChild = temp.lChild;
	//b->rChild = temp.rChild;
	//b->parent = temp.parent;
}

//void deleteMatches(matches * m, int num_bits) {
//	for (int i = 0; i < num_bits; i++) {
//
//	}
//	free(m->begin_arr);
//	free(m->end_arr);
//}


//optimum matches

void checkHeaders(Matches * matches, Match * m) {
	int length = matchLength(m);

	if (m->start > matches->max_body_start) matches->max_body_start = m->start;
	if (m->cindex > matches->max_crypt_start) matches->max_crypt_start = m->cindex;
	if (length > matches->max_length) matches->max_length = length;

	return;
}

void printOptimumMatches(FILE * map, Matches * matches, Match ** optArr, int size) {
	BitPrinter * printer = newBitPrinter(map);

	int digMaxBindex = numBinDigits(matches->max_body_start);
	int digMaxCindex = numBinDigits(matches->max_crypt_start);
	int digMaxLength = numBinDigits(matches->max_length);

	/* print headers */

	for (int i = 0; i < digMaxBindex; i++) printBit(printer, 1);	//digits needed for bindex
	printBit(printer, 0);	//spacer
	for (int i = 0; i < digMaxCindex; i++) printBit(printer, 1);	//digits needed for cindex
	printBit(printer, 0);	//spacer
	for (int i = 0; i < digMaxLength; i++) printBit(printer, 1);	//digits needed for cindex
	printBit(printer, 0);	//spacer

	/* start printing out actual matches */
	Match * curr;
	for (int i = 0; i < size; i++) {
		curr = optArr[i];
		printInt(printer, curr->start, digMaxBindex);
		printInt(printer, curr->cindex, digMaxCindex);
		printInt(printer, matchLength(curr), digMaxLength);	//print out the 3 items that define a match
	}
	flushPrinter(printer);
}


int findOptimumMatches(Matches * matches, Match ** optArr) {
	/* print out a match near the center */
	Match * center = matches->start_arr[matches->bits_covered / 2];
	printMatch(center); printf("\n");
	optArr[0] = center;
	checkHeaders(matches, center);

	int a = center->start != 0 ? findOptimumLeft(matches, center, 1, optArr) : 1;
	int b = center->end != matches->bits_covered ? findOptimumRight(matches, center, a, optArr) : a;

	return b;
}

int findOptimumRight(Matches * matches, Match * center, int i, Match ** optArr) {
	/* find the match contained within 1 to the right of the end that ends furthest to the right */
	Match * furthest = center;
	Match * curr;
	int end = center->end + 1;
	for (int k = center->start; k <= end; k++) {
		curr = matches->start_arr[k];
		if (curr->end > furthest->end) furthest = curr;
	}	//furthest is now the match that ends furthest to the right and can connect with center

	printMatch(furthest); printf("\n");
	checkHeaders(matches, furthest);
	if (optArr[i] != NULL) printf("ERROR IT WAS ALREADY FILLED\n\n\n\n\n\n\n");
	optArr[i] = furthest;
	i++;

	int n = furthest->end < matches->bits_covered - 1 ? findOptimumRight(matches, furthest, i, optArr) : i;

	return n;
}

int findOptimumLeft(Matches * matches, Match * center, int i, Match ** optArr) {
	/* find the match contained within 1 to the left of the start that starts furthest to the left */
	Match * furthest = center;
	Match * curr;
	for (int k = center->start - 1; k <= center->end; k++) {
		curr = matches->end_arr[k];
		if (curr->start < furthest->start) furthest = curr;
	}	//furthest is now the match that starts furthest to the left and can connect with center

	printMatch(furthest); printf("\n");
	checkHeaders(matches, furthest);
	if (optArr[i] != NULL) printf("ERROR IT WAS ALREADY FILLED\n\n\n\n\n\n\n");
	optArr[i] = furthest;
	i++;

	int n = furthest->start != 0 ? findOptimumLeft(matches, furthest, i, optArr) : i;

	return n;
}

void testOptimum(Match ** optArr, int optSize, Matches * matches) {
	printf("Testing the optimum matches...\n");
	int tot0 = 0;
	int tot1 = 0;
	int tot2 = 0;
	int tot3 = 0;
	for (int i = 0; i < matches->bits_covered + 1; i++) {
		int n = 0;
		for (int k = 0; k < optSize; k++) {
			if (i <= optArr[k]->end && i >= optArr[k]->start) {
				n++;
			}
		}
		if (n == 0) tot0++;
		else if (n == 1) tot1++;
		else if (n == 2) tot2++;
		else if (n == 3) tot3++;
	}
	printf("Covered by 0: %i\nCovered by 1: %i\nCovered by 2: %i\nCovered by 3: %i\nTotal: %i\n", tot0, tot1, tot2, tot3, tot0 + tot1 + tot2 + tot3);
}

int numDecDigits(unsigned int num) {
	if (num < 10) return 1;
	else if (num < 100) return 2;
	else if (num < 1000) return 3;
	else if (num < 10000) return 4;
	else if (num < 100000) return 5;
	else if (num < 1000000) return 6;
	else if (num < 10000000) return 7;
	else if (num < 100000000) return 8;
	else if (num < 1000000000) return 9;
	else if (num < INT_MAX) return 10;
	else return 0;
}

int numBinDigits(unsigned int num) {
	int digits;
	for (digits = 0; num > 0; num >>= 1) digits++;
	return digits;
}