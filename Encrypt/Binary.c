#include "stdafx.h"
#include "Binary.h"
#include "Matchings.h"
#include "IntArray.h"

Binary * newBinary(max) {
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

//void compareRange(Binary * binary, Matches * matches, int b_start, int c_start, int l) {
//	int mode = 0;  //mode 0 for neg match, 1 for null, 2 for pos match
//	int b;
//	int c;
//	int m;
//	int dur = 0;
//
//	for (int i = 0; i < l; i++) {	//run l times
//		b = binary->body[b_start + i];
//		c = binary->crypt[c_start + i];
//		m = b + c;
//
//		if (m == mode) {	//continuation
//			dur++;	//increase duration of match
//		}
//		else {	//mode is switching, determine if match should be added
//			if (dur >= 2) {	//match size of at least two
//
//
//			}
//		}
//
//	}
//}

//void deleteBinaryHandle(Binary * b) {
//	free(b_binary);
//	free(c_binary);
//}

