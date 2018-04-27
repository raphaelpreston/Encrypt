#include <stdio.h>
#include "Binary.h"
#include "Matchings.h"
#include "MatchHeap.h"

#define BUFFER_SIZE 1
#define MAX_BINARY_SIZE 2

int main()
{

	// /* create file pointers for body and crypt */
	//char body_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/body";
	//char crypt_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/crypt";
	//FILE * body = fopen(body_loc, "rb");
	//FILE * crypt = fopen(crypt_loc, "rb");
	//if (body == NULL || crypt == NULL) return 1;

	///* create a binary handle object */
	//Binary * binary = newBinary(MAX_BINARY_SIZE);
	//

	// /* read files in buffer by buffer */
	//unsigned char b_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	//unsigned char c_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	//int b_bytes_read;
	//int c_bytes_read;
	//
	//while (b_bytes_read = fread(b_buffer, sizeof(unsigned char), BUFFER_SIZE, body)) {	//for the body
	//	readInBody(binary, b_buffer, b_bytes_read);	//this finds the matchings too
	//}
	//while (c_bytes_read = fread(c_buffer, sizeof(unsigned char), BUFFER_SIZE, crypt)) {	//for the crypt
	//	readInCrypt(binary, c_buffer, c_bytes_read);
	//}

	//if (ferror(body) || ferror(crypt)) return 2;	//error checking

	// /* print binary */
	//printBinaryHandle(binary);

	//
	// /* find matchings */
	//printf("Finding matches...\n");
	//int matches_size = 21;
	//Matches * matches = newMatches(matches_size);
	//
	//printf("Performing Range Analysis... \n");
	//bodyCryptAnalysis(binary, matches);

	//printMatchesValidity(matches, binary);
	//

	//// printMatches(matches);
	/* print out most efficient matches using the MatchHeap */
	//testing MatchHeap
	printf("\n\n");
	Matches * testers = newMatches(32);

	Match * a = newMatch(5, 15, 11, 1);	//mid 10
	Match * b = newMatch(4, 6, 3, 1);	//mid 5
	Match * c = newMatch(14, 16, 3, 1);	//mid 15
	Match * d = newMatch(11, 14, 4, 1);	//mid: 12.5
	Match * n = newMatch(20, 31, 12, 1);	//mid: 20...

	addMatch(testers, a);
	addMatch(testers, b);
	addMatch(testers, c);
	addMatch(testers, d);
	addMatch(testers, n);
	//right now we are doing this seperately but it will be worked in to addMatch
	printMatches(testers);

	heap_insertMatch(testers, c);
	printf("AFTER C:\n\n"); printMatches(testers);
	heap_insertMatch(testers, b);
	printf("AFTER B:\n\n");	printMatches(testers);
	heap_insertMatch(testers, d);
	printf("AFTER D:\n\n"); printMatches(testers);
	heap_insertMatch(testers, a);
	printMatches(testers);
	heap_insertMatch(testers, n);
	printMatches(testers);
	
	

	printHeap(testers->heap);
	printMatches(testers);



	 /* close files and free memory */
	////deleteBinaryHandle(binary);
	////deleteMatches(matches)
	/*fclose(body);
	fclose(crypt);*/

    return 0;
}