#include "stdafx.h"
#include "Matchings.h"
#include "Binary.h"
#include "IntArray.h"

#define BUFFER_SIZE 1
#define MAX_BINARY_SIZE 2
int main()
{
	 /* create file pointers for body and crypt */
	char body_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/body";
	char crypt_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/crypt";
	FILE * body = fopen(body_loc, "rb");
	FILE * crypt = fopen(crypt_loc, "rb");
	if (body == NULL || crypt == NULL) return 1;

	/* create a binary handle object */
	Binary * binary = newBinary(MAX_BINARY_SIZE);
	

	 /* read files in buffer by buffer */
	unsigned char b_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	unsigned char c_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	int b_bytes_read;
	int c_bytes_read;
	
	while (b_bytes_read = fread(b_buffer, sizeof(unsigned char), BUFFER_SIZE, body)) {	//for the body
		readInBody(binary, b_buffer, b_bytes_read);	//this finds the matchings too
	}
	while (c_bytes_read = fread(c_buffer, sizeof(unsigned char), BUFFER_SIZE, crypt)) {	//for the crypt
		readInCrypt(binary, c_buffer, c_bytes_read);
	}

	if (ferror(body) || ferror(crypt)) return 2;	//error checking

	 /* print binary */
	printBinaryHandle(binary);

	
	 /* find matchings */
	printf("Finding matches...\n");
	int matches_size = 30;
	Matches * matches = newMatches(matches_size);
	
	int bodySize = 4;//binary->body->size;
	int cryptSize = 2;// binary->crypt->size;

	//first for loop
	printf("Comparing ranges: \n");
	int b_start;
	int b_end;
	int c_start;
	int c_end;
	//for (int b = 0; b < bodySize - 2; b++) {
	//	b_start = bodySize - 2 - b;
	//	b_end = bodySize - 1;
	//	c_start = 0;
	//	c_end = 1 + b;

	//	if (bodySize <= cryptSize) {
	//		printf("Body:  %i->%i\n", b_start, b_end);
	//		printf("Crypt: %i->%i\n\n", c_start, c_end);
	//	}
	//	else {
	//		printf("Body:  %i->%i\n", c_start, c_end);
	//		printf("Crypt: %i->%i\n\n", b_start, b_end);
	//	}
	//}
	//
	////second for loop
	//for (int b = 0; b <= cryptSize - bodySize; b++) {
	//	b_start = 0;
	//	b_end = bodySize - 1;
	//	c_start = b;
	//	c_end = bodySize - 1 + b;

	//	if (bodySize <= cryptSize) {
	//		printf("Body:  %i->%i\n", b_start, b_end);
	//		printf("Crypt: %i->%i\n\n", c_start, c_end);
	//	}
	//	else {
	//		printf("Body:  %i->%i\n", c_start, c_end);
	//		printf("Crypt: %i->%i\n\n", b_start, b_end);
	//	}
	//}

	////third for loop
	//for (int b = 0; b < bodySize - 2; b++) {
	//	b_start = 0;
	//	b_end = bodySize - 2 - b;
	//	c_start = cryptSize - bodySize + 1 + b;
	//	c_end = cryptSize - 1;

	//	if (bodySize <= cryptSize) {
	//		printf("Body:  %i->%i\n", b_start, b_end);
	//		printf("Crypt: %i->%i\n\n", c_start, c_end);
	//	}
	//	else {
	//		printf("Body:  %i->%i\n", c_start, c_end);
	//		printf("Crypt: %i->%i\n\n", b_start, b_end);
	//	}
	//}

	//would probably work if I made it remove negative values, but trying different approach
	
	
	//printMatches(matches);

	 /* close files and free memory */
	//deleteBinaryHandle(binary);
	//deleteMatches(matches)
	fclose(body);
	fclose(crypt);

    return 0;
}