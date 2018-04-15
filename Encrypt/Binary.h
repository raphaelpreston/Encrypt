#pragma once
#include "stdafx.h"

typedef struct Binary_Handler{
	int * body;
	int * crypt;
	int b_size;	//size in bytes
	int c_size;	
	int b_max;	//max size in bytes
	int c_max;
} Binary;

/* returns a new binary object */
Binary * newBinary();

/* reads in a buffer of body into the binary object (converts to integer array) */
void readInBody(Binary * b, unsigned char b_buffer[], int b_bytes_read);

void readInCrypt(Binary * b, unsigned char c_buffer[], int c_bytes_read);

/* adds matchings from a binary object in the given range to the matches object */
//void compareRange(Binary * b, Matches * matches, int b_start, int c_start, int l);

/* Converts an array of characters to an array of binary */
void bufferToBinary(int * binary_array, unsigned char buffer[], int bytes_read, int startingIndex);

/* prints an entire Binary Handle object */
void printBinaryHandle(Binary * b);

/* Prints a binary array to stdout */
void printBinary(int * binary_array, int length_in_bytes);
