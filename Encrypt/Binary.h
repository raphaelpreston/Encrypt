#pragma once
#include "stdafx.h"

/* Converts an array of characters to an array of binary */
void bufferToBinary(int * binary_array, unsigned char buffer[], int bytes_read);

/* Prints a binary array to stdout */
void printBinary(int * binary_array, int length_in_bytes);
