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

	/* addMatch(matches, newMatch(5, 10, 6));
	addMatch(matches, newMatch(3, 6, 4));
	addMatch(matches, newMatch(2, 12, 5));
	*/
	/*
	1 2 3 4 5 6 7 8 9 1 1 2
	  x x x x x x x x x x x (m)
			x x x x x x (curr) (8)
		x x x x (max) (4)
	*/
	Match * m = newMatch(4, 7, 4);
	Match * curr;
	for (int i = 3; i <= 10; i++) {
		for (int k = 2; k <= 6; k++) {
			curr = newMatch(i, 0, k);
			printf("The combined length of %i->%i and %i->%i is %i\n", m->start, m->end, curr->start, curr->end, testMerge(m, curr));
		}
	}

	printMatches(matches);


	 /* close files and free memory */
	free(b_binary);
	free(c_binary);
	//deleteMatches(matches)
	fclose(body);
	fclose(crypt);

    return 0;
}