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

void readInBody(Binary * b, unsigned char b_buffer[], int b_bytes_read) {
	if (!b->body) {	//first time, make some space
		int * b_binary;
		b_binary = (int *)malloc(8 * b_bytes_read * sizeof(int));
		printf("Allocating %i bytes for the first time.\n", 8 * b_bytes_read * sizeof(int));
		if (!b_binary) printf("Failed to malloc for b_binary.\n\n");
		b->body = b_binary;
		b->b_max = b_bytes_read;	//first time
	}
	else if (b->b_size + b_bytes_read > b->b_max) {	//out of memory, double the size and copy string
		printf("Allocating more memory for body (%i)...\n", 8 * sizeof(int) * b->b_max * 2);
		//make a new buffer with 2x as much space
		char * buffer;
		buffer = malloc(8 * sizeof(int) * b->b_max * 2);
		if (buffer == NULL) {
			perror("Error mallocing new buffer for body.\n\n");
		}

		//fill it with the old body content
		printBinary(b->body, b->b_size); printf("\n");
		memcpy(buffer, b->body, b->b_size*8*sizeof(int));
		printBinary(buffer, b->b_size); printf("\n");

		//free the old memory and reassign buffer
		free(b->body);
		b->body = buffer;
		printBinary(b->body, b->b_size); printf("\n");

		//update size
		b->b_max *= 2;
	}
	
	/* convert unsigned char buffers to int arrays */
	printf("Attempting to convert body at size %i\n", b->b_size);
	printBinaryHandle(b); printf("Before read in <<\n");
	bufferToBinary(b->body, b_buffer, b_bytes_read, b->b_size);	//the starting point is the current size
	printBinaryHandle(b); printf("After read in <<\n");

	/* update the size */
	b->b_size += b_bytes_read;
}

void readInCrypt(Binary * b, unsigned char c_buffer[], int c_bytes_read) {
	if (!b->crypt) {	//first time, make some space
		int * c_binary;
		c_binary = (int *)malloc(8 * c_bytes_read * sizeof(int));
		if (!c_binary) printf("Failed to malloc for c_binary.\n\n");
		b->crypt = c_binary;
		b->c_max = c_bytes_read;	//first time
	}

	/* convert unsigned char buffers to int arrays */
	printf("Attempting to convert crypt at size %i\n", b->c_size);
	bufferToBinary(b->crypt, c_buffer, c_bytes_read, b->c_size); //the starting point is the current size

	/* update the size */
	b->c_size += c_bytes_read;
}

void bufferToBinary(int * binary, unsigned char buffer[], int bytes_read, int startingIndex) {
	for (int i = 0; i < bytes_read; i++) {	//for all chars(bytes) read
		for (int k = 7; k >= 0; k--) {	//for every bit in the char(byte)
			printf("updating index at %i * 8 + (7 - %i) + (%i) * 8 = %i\n", i, k, startingIndex, i * 8 + (7 - k) + (startingIndex) * 8);
			binary[i * 8 + (7 - k) + (startingIndex) * 8] = ((buffer[i] & 1 << k) == 0) ? 0 : 1;	//assign the appropriate index the appropriate bit value
		}
	}
}

void printBinaryHandle(Binary * b) {
	printf("B: ");
	printBinary(b->body, b->b_size);
	printf("\n");
	printf("C: ");
	printBinary(b->crypt, b->c_size);
	printf("\nBody size: %i\n", b->b_size);
	printf("Crypt size: %i\n", b->c_size);
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

