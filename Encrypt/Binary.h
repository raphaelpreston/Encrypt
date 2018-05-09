#ifndef BINARY_H
#define BINARY_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "Matchings.h"
#include "IntArray.h"

//typedef struct Matches Matches;		//forward definition (what the heck is this i dunno but it works)

typedef struct Binary_Handler{
	struct IntArray * body;
	struct IntArray * crypt;
} Binary;

typedef struct Bit_Printer {	//send this individual bits or chars to write
	bool * boolArr;	//going to be size 8 ALWAYS
	int next;		//less than / equal to 8 always (index of last modified bit)
	FILE * map;
} BitPrinter;

/* returns a new binary object */
Binary * newBinary(int max);

/* returns a new bitprinter */
BitPrinter * newBitPrinter(FILE * map);

/* reads in a buffer of body into the binary object (converts to integer array) */
void readInBody(Binary * b, unsigned char b_buffer[], int b_bytes_read);

void readInCrypt(Binary * b, unsigned char c_buffer[], int c_bytes_read);

/* adds matchings from a binary object in the given range to the matches object */
void compareRange(Binary * b, struct Matches * matches, int b_start, int c_start, int l);

/* prints an entire Binary Handle object */
void printBinaryHandle(Binary * b);

/* analyzes the entire body/crypt and adds all matches to the given Matches object */
void bodyCryptAnalysis(Binary * binary, struct Matches * matches);

// printing

/* flushes and resets a printer */
void flushPrinter(BitPrinter * printer);

/* resets a printers boolean array */
void resetBool(BitPrinter * printer);

/* converts a boolean area of size 8 to an unsigned char */
unsigned char boolArrToChar(bool * arr);

/* queueus up a bit and prints it if necessary */
void printBit(BitPrinter * printer, bool bit);

#endif