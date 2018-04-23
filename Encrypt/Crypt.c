#include "stdafx.h"

#define BUFFER_SIZE 1
#define MAX_BINARY_SIZE 2
int main()
{
	 /* create file pointers for body and crypt */
	char body_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/a";
	char crypt_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/b";
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
	int matches_size = 21;
	Matches * matches = newMatches(matches_size);
	
	

	//first for loop
	printf("Performing Range Analysis... \n");
	bodyCryptAnalysis(binary, matches);




	
	printMatches(matches);

	 /* close files and free memory */
	//deleteBinaryHandle(binary);
	//deleteMatches(matches)
	fclose(body);
	fclose(crypt);

    return 0;
}