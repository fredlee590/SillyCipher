/*
 * sillyXcryptionFunc.c
 *
 * Contains the encryption and support functions for encryption
 */

#include <openssl/md5.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXCHAR '~'
#define APOS 39
#define MINCHAR ' '
#define RANGE (MAXCHAR - MINCHAR + 1)

char* sillyXcrypt(char* keyword, char* toXcrypt, char direction, char* file_to_read)
{
	int i;
	MD5_CTX c;
	unsigned char out[MD5_DIGEST_LENGTH];

	// set up defaults
	char* result = malloc(sizeof(char) * strlen(toXcrypt) + 1);

	// compute md5sum of keyword
	unsigned long keyword_len = strlen(keyword);
	unsigned int toXcrypt_len = strlen(toXcrypt);

	strcpy(result, toXcrypt);

	MD5_Init(&c);
	MD5_Update(&c, keyword, keyword_len);

	MD5_Final(out, &c);

	// walk down each part of the md5sum
	for(i = 0; i < toXcrypt_len; i++)
	{
		// compute offset from md5sum part
		char sign;
		unsigned val = out[i % MD5_DIGEST_LENGTH];
		unsigned char offset = val & 0x1F;
		//if((val & 0x80) ^ (val & 0x40))
		if(val & 0x20)
		{
			sign = direction;
		}
		else
		{
			sign = -direction;
		}

		#ifdef DEBUG
		printf("newChar = %c + (%d * %d)\n", result[i], sign, offset);
		#endif

		unsigned char oldChar = result[i];

		// keep non-printed characters the same (eg. end of file chars). todo: determine if this is feasible to keep.
		if(oldChar < MINCHAR || oldChar > MAXCHAR)
		{
			continue;
		}

		unsigned char newChar = oldChar + (sign * offset);

		// Bounds checking. Wrap around
		if(newChar < MINCHAR)
		{
			#ifdef DEBUG
			printf("newChar %c (%d) is less than '!' - ", newChar, newChar);
			#endif
			newChar += RANGE;
			#ifdef DEBUG
			printf("adjusted to %c (%d)\n", newChar, newChar);
			#endif
		}
		else if(newChar > MAXCHAR)
		{
			#ifdef DEBUG
			printf("newChar %c (%d) is greater than '~' - ", newChar, newChar);
			#endif
			newChar -= RANGE;
			#ifdef DEBUG
			printf("adjusted to %c (%d)\n", newChar, newChar);
			#endif
		}

		#ifdef DEBUG
		printf("%d %c %c %d\n", i, result[i], newChar, newChar);
		#endif

		// append to new string
		result[i] = newChar;

		// special case: Need to reserve apostrophes for use on the command line
		// todo: make apostrophes okay for files?
		// if we land here, go one more round
		if(newChar == APOS && !file_to_read)
		{
			i--;
			continue;
		}
	}

	return result;
}
