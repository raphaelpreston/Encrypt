#pragma once
#include "stdafx.h"

typedef struct Binary_Handler{
	int * body;
	int * crypt;
	int b_size;
	int c_size;
} Binary;

/* returns a new binary object */
Binary * newBinary();

/* reads in a buffer of body and crypt into the binary object (converts to integer array) */
void readInBinary(Binary * b, unsigned char b_buffer[], unsigned char c_buffer[], int b_bytes_read, int c_bytes_read);

/* Converts an array of characters to an array of binary */
void bufferToBinary(int * binary_array, unsigned char buffer[], int bytes_read, int startingIndex);

/* prints an entire Binary Handle object */
void printBinaryHandle(Binary * b);

/* Prints a binary array to stdout */
void printBinary(int * binary_array, int length_in_bytes);
