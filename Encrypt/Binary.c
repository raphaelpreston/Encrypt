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
		printf("\nAnalyzing b: %i with c: %i\n", b_start + i, c_start + i);
		m = binary->body->arr[b_start + i] + binary->crypt->arr[c_start + i] == 1 ? 2 : 1; //if it's 2 or 0, set to pos.  If it's 1 then its neg.
		printf(" Set m to %s, and mode is currently %s\n", m == 0 ? "NULL" : m == 1 ? "POS" : "NEG", mode == 0 ? "NULL" : mode == 1 ? "POS" : "NEG");
		if (m == mode) {	//continuation
			printf(" Continuation detected, duration is now %i.\n", i-m_start+1);
		}
		else {	//mode is switching, determine if match should be added
			if (i-m_start >= 2) {	//add match
				Match * match = newMatch(b_start + m_start, c_start + m_start, i-m_start, mode);
				printf(" Adding new %s match: ", mode==0?"NULL":mode==1?"POS":"NEG"); printMatch(match); printf("\n");
				addMatch(matches, match);
			}
			else printf(" Match not sufficient length (%i).\n", i-m_start);
			m_start = i;	//starting looking at new match
			mode = m;
		}
	}
	//check last one
	if (l - m_start >= 2) {	//add match
		Match * match = newMatch(b_start + m_start, c_start + m_start, l - m_start, m);
		printf(" Adding new %s match: ", mode == 0 ? "NULL" : mode == 1 ? "POS" : "NEG"); printMatch(match); printf("\n");
		addMatch(matches, match);
	}
	else printf(" Match not sufficient length (%i).\n", l - m_start);
}

//void deleteBinaryHandle(Binary * b) {
//	free(b_binary);
//	free(c_binary);
//}

