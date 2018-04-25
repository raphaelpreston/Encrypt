#include <stdio.h>
#include "Binary.h"
#include "Matchings.h"

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
	int matches_size = 21;
	Matches * matches = newMatches(matches_size);
	
	printf("Performing Range Analysis... \n");
	bodyCryptAnalysis(binary, matches);

	printMatchesValidity(matches, binary);

	// printMatches(matches);
	/* print out most efficient matches using the MatchHeap */
	//testing middle
	printf("6: %i\n", middle(newMatch(4, 9, 6, 1), -1));
	printf("7: %i\n", middle(newMatch(4, 9, 6, 1), 1));

	printf("6: %i\n", middle(newMatch(4, 8, 5, 1), 1));



	 /* close files and free memory */
	//deleteBinaryHandle(binary);
	//deleteMatches(matches)
	fclose(body);
	fclose(crypt);

    return 0;
}