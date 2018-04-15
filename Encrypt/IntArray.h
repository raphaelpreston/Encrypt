#pragma once
#include "stdafx.h"

/* structs */

typedef struct IntArray {
	int * arr;
	int size;
	int max;
} IntArr;

/* returns a new intArr with size 0, max = max, and arr = malloced */
IntArr * newIntArr(int max);

/* dynamically append a new integer to the array */
void IntArr_append(IntArr * intArr, int num);

/* print the array */
void printIntArr(IntArr * intArr);
