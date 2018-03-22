#include "stdafx.h"
#include "Binary.h"

void bufferToBinary(int * binary, unsigned char buffer[], int bytes_read) {
	for (int i = 0; i < bytes_read; i++) {	//for all chars(bytes) read
		for (int k = 7; k >= 0; k--) {	//for every bit in the char(byte)
			binary[i * 8 + (7 - k)] = ((buffer[i] & 1 << k) == 0) ? 0 : 1;	//assign the appropriate index the appropriate bit value
		}
	}
}

void printBinary(int * binary_array, int length) {
	int l = length * 8;
	for (int i = 0; i < l; i++) {
		printf("%i%s", binary_array[i], i%8==7?" ":"");
	}
}