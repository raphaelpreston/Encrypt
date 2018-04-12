#pragma once
#include "stdafx.h"

typedef struct Binary {
	int * body;
	int * crypt;
	int b_size;
	int c_size;
} Binary;

/* returns a new binary object */
Binary * newBinary(unsigned char b_buffer, unsigned char c_buffer, unsigned char b_bytes_read, unsigned char c_bytes_read);

/* Converts an array of characters to an array of binary */
void bufferToBinary(int * binary_array, unsigned char buffer[], int bytes_read);

/* Prints a binary array to stdout */
void printBinary(int * binary_array, int length_in_bytes);
