#include "stdafx.h"
#include "Binary.h"
#include "Matchings.h"
#include "IntArray.h"

Binary * newBinary(int max) {
	/* allocate space */
	Binary * b = (Binary *)malloc(sizeof(Binary));
	if (b) {
		b->body = newIntArr(max);
		b->crypt = newIntArr(max);
		return b;
	}
	else {
		printf("Failed to malloc space for new binary object.");
		return NULL;
	}
}

void readInBody(Binary * b, unsigned char b_buffer[], int b_bytes_read) {
	IntArr_readInBuffer(b->body, b_buffer, b_bytes_read);
}

void readInCrypt(Binary * b, unsigned char c_buffer[], int c_bytes_read) {
	IntArr_readInBuffer(b->crypt, c_buffer, c_bytes_read);
}

void printBinaryHandle(Binary * b) {
	printf("B: ");
	printIntArr(b->body);
	printf("\n");
	printf("C: ");
	printIntArr(b->crypt);
	printf("\n\n");
}

// matching functions

void compareRange(Binary * binary, Matches * matches, int b_start, int c_start, int l) {
	int mode = 0;  //mode 0 for NULL, mode 1 for POS, mode 2 for NEG
	int m;
	int m_start = 0;

	for (int i = 0; i < l; i++) {	//run l times
									//printf("\nAnalyzing b: %i with c: %i\n", b_start + i, c_start + i);
		m = binary->body->arr[b_start + i] + binary->crypt->arr[c_start + i] == 1 ? 2 : 1; //if it's 2 or 0, set to pos.  If it's 1 then its neg.
																						   //printf(" Set m to %s, and mode is currently %s\n", m == 0 ? "NULL" : m == 1 ? "POS" : "NEG", mode == 0 ? "NULL" : mode == 1 ? "POS" : "NEG");
		if (m == mode) {	//continuation
							//printf(" Continuation detected, duration is now %i.\n", i-m_start+1);
		}
		else {	//mode is switching, determine if match should be added
			if (i - m_start >= 2) {	//add match
				Match * match = newMatch(b_start + m_start, c_start + m_start, i - m_start, mode);
				//printf(" Adding new %s match: ", mode==0?"NULL":mode==1?"POS":"NEG"); printMatch(match); printf("\n");
				addMatch(matches, match);
			}
			//else printf(" Match not sufficient length (%i).\n", i-m_start);
			m_start = i;	//starting looking at new match
			mode = m;
		}
	}
	//check last one
	if (l - m_start >= 2) {	//add match
		Match * match = newMatch(b_start + m_start, c_start + m_start, l - m_start, m);
		//printf(" Adding new %s match: ", mode == 0 ? "NULL" : mode == 1 ? "POS" : "NEG"); printMatch(match); printf("\n");
		addMatch(matches, match);
	}
	//else printf(" Match not sufficient length (%i).\n", l - m_start);
}

void bodyCryptAnalysis(Binary * binary, Matches * matches) {
	//treat the smaller one as the body
	int a = binary->body->size;
	int b = binary->crypt->size;
	/*int bodySize = binary->body->size < binary->crypt->size ? binary->body->size : binary->crypt->size;
	int cryptSize = binary->body->size < binary->crypt->size ? binary->crypt->size : binary->body->size;*/

	int bodySize = a <= b ? a : b;
	int cryptSize = a <= b ? b : a;

	int b_start = bodySize - 2;
	int b_end = bodySize - 1;
	int c_start = 0;
	int c_end = 1;

	//first while loop
	while (b_start > 0) {
		printf("Body:  %i->%i\n", b_start, b_end);
		printf("Crypt: %i->%i\n\n", c_start, c_end);
		compareRange(binary, matches, b_start, c_start, b_end - b_start + 1);
		b_start--;
		c_end++;
	}
	//second while loop
	while (c_end < cryptSize - 1) {
		printf("Body:  %i->%i\n", b_start, b_end);
		printf("Crypt: %i->%i\n\n", c_start, c_end);
		compareRange(binary, matches, b_start, c_start, b_end - b_start + 1);
		c_start++;
		c_end++;
	}
	//third while loop
	//printf("After: b_start: %i, b_end: %i, c_start: %i, c_end: %i\n", b_start, b_end, c_start, c_end);

	while (c_start < cryptSize - 1) {
		printf("Body:  %i->%i\n", b_start, b_end);
		printf("Crypt: %i->%i\n\n", c_start, c_end);
		compareRange(binary, matches, b_start, c_start, b_end - b_start + 1);
		b_end--;
		c_start++;
	}

}

//void deleteBinaryHandle(Binary * b) {
//	free(b_binary);
//	free(c_binary);
//}

