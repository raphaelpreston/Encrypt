#include <stdio.h>
#include "stdafx.h"

void bufferToBinary(int * binary, unsigned char buffer[], int bytes_read) {
	for (int i = 0; i < bytes_read; i++) {	//for all chars(bytes) read
		for (int k = 7; k >= 0; k--) {	//for every bit in the char(byte)
			//if ((buffer[i] & 1 << k) == 0) binary[i*8 + (7-k)] = 0;	//assign to the array its equivalent bit value
			//else binary[i * 8 + (7 - k)] = 1;
			binary[i * 8 + (7 - k)] = ((buffer[i] & 1 << k) == 0) ? 0 : 1;
		}
	}
}

void printBinary(int * binary_array, int length) {
	int l = length * 8;
	for (int i = 0; i < l; i++) {
		printf("%i%s", binary_array[i], i%8==7?" ":"");
	}
}