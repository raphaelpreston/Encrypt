#include "stdafx.h"
#include "Matchings.h"
#include "Binary.h"

#define BUFFER_SIZE 256

int main()
{
	 /* create file pointers for body and crypt */
	char body_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/body";
	char crypt_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/crypt";
	FILE * body = fopen(body_loc, "rb");
	FILE * crypt = fopen(crypt_loc, "rb");
	if (body == NULL || crypt == NULL) return 1;
	
	 /* read files in */
	unsigned char b_buffer[BUFFER_SIZE];
	unsigned char c_buffer[BUFFER_SIZE];
	int b_bytes_read = 0;
	int c_bytes_read = 0;
	b_bytes_read = fread(b_buffer, sizeof(unsigned char), BUFFER_SIZE, body);
	c_bytes_read = fread(c_buffer, sizeof(unsigned char), BUFFER_SIZE, crypt);
	if (ferror(body) || ferror(crypt)) return 2;	//error checking

	/* create a binary object */
	Binary * binary = newBinary();
	readInBinary(binary, b_buffer, c_buffer, b_bytes_read, c_bytes_read);

	 /* print binary */
	printBinaryHandle(binary);


	 /* find matchings */
	int matches_size = 1000;
	Matches * matches = newMatches(matches_size);



	 /* close files and free memory */
	//deleteBinaryHandle(binary);
	//deleteMatches(matches)
	fclose(body);
	fclose(crypt);

    return 0;
}