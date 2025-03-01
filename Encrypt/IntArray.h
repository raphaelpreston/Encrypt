#ifndef INTARRAY_H
#define INTARRAY_H

/* structs */

typedef struct IntArray {
	int * arr;
	int size;
	int max;
} IntArr;

/* returns a new intArr with size 0, max = max, and arr = malloced */
IntArr * newIntArr(int max);

/* reads in a buffer array of characters into the int array by converting each byte into 8 integers */
void IntArr_readInBuffer(IntArr * intArr, unsigned char buffer[], int bytes_read);

/* print the array */
void printIntArr(IntArr * intArr);

#endif