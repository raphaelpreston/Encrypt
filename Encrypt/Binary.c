#include "stdafx.h"
#include "Binary.h"

//working here
Binary * newBinary() {
	/* allocate space */
	Binary * b = (Binary *)malloc(sizeof(Binary));
	if (b) {
		b->body = NULL;
		b->crypt = NULL;
		b->b_size = 0;
		b->c_size = 0;
		return b;
	}
	else {
		printf("Failed to malloc space for new binary object.");
		return NULL;
	}
}

void readInBinary(Binary * b, unsigned char b_buffer, unsigned char c_buffer, int b_bytes_read, int c_bytes_read) {
	

	 /* convert unsigned char buffers to int arrays */
	bufferToBinary(b->body, b_buffer, b_bytes_read, b->b_size);	//the starting point is the current size
	bufferToBinary(b->crypt, c_buffer, c_bytes_read, b->c_size);

	/* update the size */
	b->b_size += b_bytes_read;
	b->c_size += c_bytes_read;
	
}

void bufferToBinary(int * binary, unsigned char buffer[], int bytes_read, int startingIndex) {
	for (int i = 0; i < bytes_read; i++) {	//for all chars(bytes) read
		for (int k = 7; k >= 0; k--) {	//for every bit in the char(byte)
			binary[i * 8 + (7 - k) + startingIndex] = ((buffer[i] & 1 << k) == 0) ? 0 : 1;	//assign the appropriate index the appropriate bit value
		}
	}
}

void printBinary(int * binary_array, int length) {
	int l = length * 8;
	for (int i = 0; i < l; i++) {
		printf("%i%s", binary_array[i], i%8==7?" ":"");
	}
}



