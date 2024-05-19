// Frederick Lee
// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "sillyCipherFunc.h"

void usage(void)
{
	printf("SillyCipher is a command line utility that encrypts strings with dynamically generated Vigenère keys.\n");
	printf("\n");
	printf("Usage:\n");
	// TODO: make MESSAGE into MESSAGE|FILE
	printf("    sillycipher [OPTIONS] KEYWORD MESSAGE\n");
 	printf("\n");
 	printf("Options:\n");
	printf("    -d, --decrypt           Decrypts input MESSAGE instead of encrypting.\n");
	printf("    -f, --file              File from which to read in source text\n");
	printf("    -o, --output            File to which to write resultant text\n");
	printf("    -h, --help              Print this help\n");
}

int main(int argc, char** argv)
{
	int opt;
	int option_index = 0;

	int curarg = 1;
	char direction = ENCRYPT;
	char* file_to_read = NULL;
	char* file_to_write = NULL;

	static struct option long_options[] = {
		{"decrypt",	no_argument,		0,	'd'	},
		{"file",	required_argument,	0,	'f'	},
		{"output",	required_argument,	0,	'o'	},
		{"help",	no_argument,		0,	'h'	},
		{0,		0,			0,	0	}
	};

	// process options
	while((opt = getopt_long(argc, argv, "df:o:h", long_options, &option_index)) != -1)
	{
		switch(opt)
		{
			case 'd':
				// decrypt instead of default encrypt
				direction = DECRYPT;
				break;
			case 'f':
				// decrypt file instead
				file_to_read = optarg;
				curarg++; // too many args?
				break;
			case 'o':
				// output to file named arg instead of stdout
				file_to_write = optarg;
				curarg++;
				break;
			case 'h':
				// help
				usage();
				return 0;
			default:
				break;
		}

		curarg++; // too many args?
	}

	char* keyword = argv[curarg++];
	if(!keyword)
	{
		fputs("Keyword from which to generate Vigenère key required!\n", stderr);

		return 1;
	}

	char* toXcrypt;
	if(file_to_read == NULL)
	{
		toXcrypt = argv[curarg++];

		if(curarg != argc)
		{
			fputs("Additional arguments unexpectedly detected!\n", stderr);
			return 2;
		}
	}
	else
	{
		long fSize;

		// load file into buffer
		FILE* file = fopen(file_to_read, "r");

		if(file == NULL) // todo: replace with assert
		{
			fputs("File error\n", stderr);
			exit(1);
		}

		// cycle through entire file
		// todo: do this N char by N char
		// first, find file size
		fseek(file, 0, SEEK_END);
		fSize = ftell(file);
		rewind(file);

		// load file to string
		toXcrypt = (char*)malloc(sizeof(char) * fSize);

		if(fread(toXcrypt, 1, fSize, file) != fSize)
		{
			fputs("Reading error\n", stderr);
			exit(2);
		}

		// done with file
		fclose(file);
	}

	// print string to encrypt or decrypt, whatever it was
	char* XcryptedStr = sillyXcrypt(keyword, toXcrypt, direction, file_to_read);
	if(file_to_write)
	{
		FILE* file = fopen(file_to_write, "w");
		fwrite(XcryptedStr, sizeof(char), strlen(XcryptedStr), file);
		fclose(file);
	}
	else
	{
		printf("%s\n", XcryptedStr);
	}
	free(XcryptedStr);

	return 0;
}
