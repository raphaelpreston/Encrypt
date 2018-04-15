//dynamic integer array to help with various aspects

#include "stdafx.h"
#include "IntArray.h"

IntArr * newIntArr(int max) {
	IntArr * intArr = (IntArr *)malloc(sizeof(IntArr));
	int * arr = (int *)malloc(max * sizeof(int));

	/* fill */
	intArr->arr = arr;
	intArr->max = max;
	intArr->size = 0;

	return intArr;

}

void IntArr_append(IntArr * intArr, int num) {
	if (intArr->size == intArr->max) {	//no room, double the memory
		int * buffer;
		buffer = malloc(intArr->max * sizeof(int) * 2);
		if (!buffer) perror("Error mallocing new buffer for appending.\n\n\n");
		
		//fill it with old content
		memcpy(buffer, intArr->arr, intArr->size * sizeof(int));

		//free old memory, reasign buffer
		free(intArr->arr);
		intArr->arr = buffer;

		//update max
		intArr->max *= 2;
	}
	else if (intArr->size > intArr->max) printf("U messed up!!\n\n\n");

	//guaranteed enough memory, time to add it

	intArr->arr[intArr->size] = num;
	intArr->size++;
}

void printIntArr(IntArr * intArr) {
	for (int i = 0; i < intArr->size; i++) {
		printf("%i", intArr->arr[i]);
	}
	printf("\nSize: %i\nMax: %i\n", intArr->size, intArr->max);
}