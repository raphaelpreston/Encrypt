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


	 /* convert char buffer to int array */
	int * b_binary;
	int * c_binary;
	b_binary = (int *)malloc(8*b_bytes_read*sizeof(int));
	c_binary = (int *)malloc(8*c_bytes_read*sizeof(int));

	bufferToBinary(b_binary, b_buffer, b_bytes_read);
	bufferToBinary(c_binary, c_buffer, c_bytes_read);

	 /* print binary */
	printf("Body: ");
	printBinary(b_binary, b_bytes_read);
	printf("\n");
	printf("Crypt: ");
	printBinary(c_binary, c_bytes_read);
	printf("\n\n");


	 /* find matchings */
	int matches_size = 21;
	Matches * matches = newMatches(matches_size);

	/* testing */
	/*srand(time(NULL));
	int bindex;
	int cindex;
	int length;*/
	Match * ms[3];
	ms[0] = newMatch(2, 1, 3);
	ms[1] = newMatch(3, 2, 4);
	ms[2] = newMatch(1, 5, 2);
	printMatch(merge(ms, 3)); printf("\n");
	ms[0] = newMatch(2, 1, 3);
	ms[2] = newMatch(3, 2, 4);
	ms[1] = newMatch(1, 5, 2);
	printMatch(merge(ms, 3)); printf("\n");
	ms[1] = newMatch(2, 1, 3);
	ms[2] = newMatch(3, 2, 4);
	ms[0] = newMatch(1, 5, 2);
	printMatch(merge(ms, 3)); printf("\n");
	ms[1] = newMatch(2, 1, 3);
	ms[0] = newMatch(3, 2, 4);
	ms[2] = newMatch(1, 5, 2);
	printMatch(merge(ms, 3)); printf("\n");
	ms[2] = newMatch(2, 1, 3);
	ms[0] = newMatch(3, 2, 4);
	ms[1] = newMatch(1, 5, 2);
	printMatch(merge(ms, 3)); printf("\n");
	ms[2] = newMatch(2, 1, 3);
	ms[1] = newMatch(3, 2, 4);
	ms[0] = newMatch(1, 5, 2);
	printMatch(merge(ms, 3)); printf("\n\n\n");

	ms[0] = newMatch(1, 15, 2);
	ms[1] = newMatch(3, 12, 3);
	ms[2] = newMatch(6, 8, 4);
	printMatch(merge(ms, 3)); printf("\n");



	printf(" \n");

	/*addMatch(matches, newMatch(5, 10, 6));
	addMatch(matches, newMatch(3, 6, 4));
	addMatch(matches, newMatch(2, 12, 5));*/
	//printMatches(matches);


	 /* close files and free memory */
	free(b_binary);
	free(c_binary);
	//deleteMatches(matches)
	fclose(body);
	fclose(crypt);

    return 0;
}