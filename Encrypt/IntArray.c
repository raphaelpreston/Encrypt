//dynamic integer array to help with various aspects

#include "stdafx.h"
#include "IntArray.h"

#define UPLE_CONSTANT 2	//how much to multiply the new memory by each time it runs out of space

IntArr * newIntArr(int max) {
	IntArr * intArr = (IntArr *)malloc(sizeof(IntArr));
	int * arr = (int *)malloc(max * sizeof(int));

	/* fill */
	intArr->arr = arr;
	intArr->max = max;
	intArr->size = 0;

	return intArr;

}

void IntArr_readInBuffer(IntArr * intArr, unsigned char buffer[], int bytes_read) {
	if (intArr->size + bytes_read * 8 > intArr->max) {	//not enough memory

		int numInts = intArr->max;
		while (numInts < intArr->size + bytes_read * 8) numInts *= UPLE_CONSTANT;
		 //numInts is new number of ints to hold
		int * buffer;
		buffer = malloc(numInts * sizeof(int));
		if (!buffer) printf("Error mallocing %i for ints (%i bytes) buffer for reading in buffer.\n", numInts, numInts*sizeof(int));
		else printf("Malloc successful for %i ints (%i bytes) buffer for reading in buffer.\n", numInts, numInts * sizeof(int));
		//fill it with old content
		if (intArr->size != 0) memcpy(buffer, intArr->arr, intArr->size * sizeof(int)); 

		//free old memory, reasign buffer
		free(intArr->arr);
		intArr->arr = buffer;

		//update max
		intArr->max = numInts;
	}

	/* now guaranteed enough memory, time to convert and add */
	for (int i = 0; i < bytes_read; i++) {	//for all chars(bytes) read
		for (int k = 7; k >= 0; k--) {	//for every bit in the char(byte)
			intArr->arr[i * 8 + (7 - k) + (intArr->size)] = ((buffer[i] & 1 << k) == 0) ? 0 : 1;	//assign the appropriate index the appropriate bit value
		}
	}
	
	/* update size */
	intArr->size += 8 * bytes_read;
}

void printIntArr(IntArr * intArr) {
	for (int i = 0; i < intArr->size; i++) {
		printf("%s%i", i%8==0 && i!=0?" ":"", intArr->arr[i]);
	}
	printf(" (Size: %i, Max: %i)", intArr->size, intArr->max);
}