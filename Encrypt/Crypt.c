#include <stdio.h>
#include <time.h>
#include "Binary.h"
#include "Matchings.h"
#include "MatchHeap.h"
#include "IntArray.h"

#define BUFFER_SIZE 1
#define MAX_BINARY_SIZE 2
#define MATCHES_SIZE 10

int main()
{
	/* testing more bit printing with ints */
	char map_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/MAP.txt";
	FILE * map = fopen(map_loc, "w+");

	BitPrinter * printer = newBitPrinter(map);
	int num = ((1<<10) + 1);
	printInt(printer, num, numBinDigits(num));

	flushPrinter(printer);
	
	//////// /* create file pointers for body and crypt */
	////////char body_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/longishbody";
	////////char crypt_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/longishcrypt";
	////////char map_loc[] = "C:/Users/IAMFRANK/source/repos/Encrypt2/Encrypt/MAP.txt";

	////////FILE * body = fopen(body_loc, "rb");
	////////FILE * crypt = fopen(crypt_loc, "rb");
	////////FILE * map = fopen(map_loc, "w+");

	////////if (body == NULL || crypt == NULL) return 1;

	/////////* create a binary handle object */
	////////Binary * binary = newBinary(MAX_BINARY_SIZE);
	////////

	//////// /* read files in buffer by buffer */
	////////unsigned char b_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	////////unsigned char c_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	////////int b_bytes_read;
	////////int c_bytes_read;
	////////
	////////while (b_bytes_read = fread(b_buffer, sizeof(unsigned char), BUFFER_SIZE, body)) {	//for the body
	////////	readInBody(binary, b_buffer, b_bytes_read);	//this finds the matchings too
	////////}
	////////while (c_bytes_read = fread(c_buffer, sizeof(unsigned char), BUFFER_SIZE, crypt)) {	//for the crypt
	////////	readInCrypt(binary, c_buffer, c_bytes_read);
	////////}

	////////if (ferror(body) || ferror(crypt)) return 2;	//error checking

	//////// /* print binary */
	////////printBinaryHandle(binary);
	////////// clock_t begin, end;
	////////// begin = clock();

	//////// /* find matchings */
	////////printf("Finding matches...\n");
	////////Matches * matches = newMatches(MATCHES_SIZE);
	////////bodyCryptAnalysis(binary, matches);

	////////printf("Matches found...\n");
	//////////printMatches(matches);
	//////////printMatchesValidity(matches, binary);

	////////// print out optimum matches
	////////printf("Optimum matches:\n");
	////////Match ** optArr = (Match **)calloc(502, sizeof(Match *));


	////////// now we have all matches from ALL FILES, time to find the optimum ones

	////////int numMatches = findOptimumMatches(matches, optArr);	//find the optimum matches, also sets matches header objects

	////////printf("\nThe entire body can be covered with %i matches.\n", numMatches);
	////////testOptimum(optArr, 502, matches);

	////////// now print them
	////////printOptimumMatches(map, matches, optArr, 502);	//eventually encorporate optArr into matches object
	////////printf("Matches Headers:\nMax Bindex: %i [%i]\nMax Cindex: %i [%i]\nMax Length: %i [%i]\n", matches->max_body_start, numBinDigits(matches->max_body_start), matches->max_crypt_start, numBinDigits(matches->max_crypt_start), matches->max_length, numBinDigits(matches->max_length));

	////////// end = clock();
	//////////double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	//////////printf("Elapsed: %f\n", time_spent);

	////////
	/* checking that it worked */
	fclose(map);
	fopen(map_loc, "rb");
	/* create a binary handle object */
	IntArr * intArr = newIntArr(1000);

	unsigned char map_buffer[BUFFER_SIZE * sizeof(unsigned char)];
	int map_bytes_read;

	while (map_bytes_read = fread(map_buffer, sizeof(unsigned char), BUFFER_SIZE, map)) {
		IntArr_readInBuffer(intArr, map_buffer, map_bytes_read);
	}

	printIntArr(intArr);
	printf("\n");

	 /* close files and free memory */
	//deleteBinaryHandle(binary);
	//deleteMatches(matches)
	// fclose(body);
	// fclose(crypt);
	fclose(map);

    return 0;
}