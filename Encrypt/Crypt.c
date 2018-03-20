#include "stdafx.h"
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 256

int main()
{
		/* create file pointers for body and crypt */
	char body_loc[] = "C:/Users/IAMFRANK/Documents/Visual Studio 2017/Projects/Encrypter/git/gitclone/Encrypt/body";
	char crypt_loc[] = "C:/Users/IAMFRANK/Documents/Visual Studio 2017/Projects/Encrypter/git/gitclone/Encrypt/crypt";
	FILE * body = fopen(body_loc, "rb");
	FILE * crypt = fopen(crypt_loc, "rb");
	if (body == NULL || crypt == NULL) return 1;

		/* read files in */
	unsigned char b_buffer[BUFFER_SIZE];
	unsigned char c_buffer[BUFFER_SIZE];
	size_t b_bytes_read = 0;
	size_t c_bytes_read = 0;
	b_bytes_read = fread(b_buffer, sizeof(unsigned char), BUFFER_SIZE, body);
	c_bytes_read = fread(c_buffer, sizeof(unsigned char), BUFFER_SIZE, crypt);
	if (ferror(body) || ferror(crypt)) return 2;

	printf("Body: ");
	for (int i = 0; i < b_bytes_read; i++) {
		printf("%c", b_buffer[i]);
	}
	printf("\n");

	printf("Crypt: ");
	for (int i = 0; i < c_bytes_read; i++) {
		printf("%c", c_buffer[i]);
	}

	printf("\n");
	
	




	

		/* close files */
	fclose(body);
	fclose(crypt);

    return 0;
}