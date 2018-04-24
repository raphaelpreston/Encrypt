#ifndef BINARY_H
#define BINARY_H

#include <stdlib.h>

#include "Matchings.h"
#include "IntArray.h"

//typedef struct Matches Matches;		//forward definition (what the heck is this i dunno but it works)

typedef struct Binary_Handler{
	struct IntArray * body;
	struct IntArray * crypt;
} Binary;

/* returns a new binary object */
Binary * newBinary(int max);

/* reads in a buffer of body into the binary object (converts to integer array) */
void readInBody(Binary * b, unsigned char b_buffer[], int b_bytes_read);

void readInCrypt(Binary * b, unsigned char c_buffer[], int c_bytes_read);

/* adds matchings from a binary object in the given range to the matches object */
void compareRange(Binary * b, struct Matches * matches, int b_start, int c_start, int l);

/* prints an entire Binary Handle object */
void printBinaryHandle(Binary * b);

/* analyzes the entire body/crypt and adds all matches to the given Matches object */
void bodyCryptAnalysis(Binary * binary, struct Matches * matches);

#endif