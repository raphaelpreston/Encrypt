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
	//Binary * binary = newBinary(MAX_BINARY_SIZE);
	//

	// /* read files in buffer by buffer */
	//unsigned char b_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	//unsigned char c_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	//int b_bytes_read;
	//int c_bytes_read;
	//
	//while (b_bytes_read = fread(b_buffer, sizeof(unsigned char), BUFFER_SIZE, body)) {	//for the body
	//	readInBody(binary, b_buffer, b_bytes_read);
	//}
	//while (c_bytes_read = fread(c_buffer, sizeof(unsigned char), BUFFER_SIZE, crypt)) {	//for the crypt
	//	readInCrypt(binary, c_buffer, c_bytes_read);
	//}

	//if (ferror(body) || ferror(crypt)) return 2;	//error checking

	// /* print binary */
	//printBinaryHandle(binary);


	 /* find matchings */
	printf("Finding matches...\n");
	int matches_size = 1000;
	Matches * matches = newMatches(matches_size);

	//testing compareRange
	// compareRange(binary, matches, 0, 8);

	//testing IntArr
	IntArr * intArr = newIntArr(2);
	unsigned char buffer[] = {'a', 'b', 'c', 'd', 'e', 'f'};
	int bytes_read = 10;
	IntArr_readInBuffer(intArr, buffer, bytes_read);

	printIntArr(intArr);

	 /* close files and free memory */
	//deleteBinaryHandle(binary);
	//deleteMatches(matches)
	fclose(body);
	fclose(crypt);

    return 0;
}