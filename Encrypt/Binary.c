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

void readInBinary(Binary * b, unsigned char b_buffer[], unsigned char c_buffer[], int b_bytes_read, int c_bytes_read) {
	if (!b->body && !b->crypt) {	//first time, make some space
		int * b_binary;
		int * c_binary;
		b_binary = (int *)malloc(8 * b_bytes_read * sizeof(int));
		c_binary = (int *)malloc(8 * c_bytes_read * sizeof(int));
		if (!b_binary || !c_binary) printf("Failed to malloc for b_binary or c_binary.\n\n");
		b->body = b_binary;
		b->crypt = c_binary;
	}
	else if ((b->body && !b->crypt) || (!b->body && b->crypt)) printf("You messed up lol\n");

	/* convert unsigned char buffers to int arrays */
	printf("Attempting to convert at size %i\n", b->b_size);
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

void printBinaryHandle(Binary * b) {
	printf("B: ");
	printBinary(b->body, b->b_size);
	printf("\n");
	printf("C: ");
	printBinary(b->crypt, b->c_size);
	printf("\n\n");
}

void printBinary(int * binary_array, int length) {
	int l = length * 8;
	for (int i = 0; i < l; i++) {
		printf("%i%s", binary_array[i], i%8==7?" ":"");
	}
}

//void deleteBinaryHandle(Binary * b) {
//	free(b_binary);
//	free(c_binary);
//}

