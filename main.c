// Frederick Lee
// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "sillyCipherFunc.h"

void printHelp()
{
	printf("\n");
	printf("SillyCipher is a fun project to quickly encrypt strings\n");
	printf("\n");
	printf("It was originally conceived for simple, single words like passwords, but was later adapted to\n");
	printf("encrypt files.\n");
	printf("Usage:\n");
	printf("\tHahahahaha You didn't think it would be that easy, did you!?\n");
}

int main(int argc, char** argv)
{
	int opt;
	int option_index = 0;

	int curarg = 1;
	char* keyword = NULL;
	char direction = ENCRYPT;
	char* file_to_read = NULL;
	char* file_to_write = NULL;

	static struct option long_options[] = {
		{"decrypt",	no_argument,		0,	'd'	},
		{"keyword",	required_argument,	0,	'k'	},
		{"help",	no_argument,		0,	'h'	},
		{"file",	required_argument,	0,	'f'	},
		{"output",	required_argument,	0,	'o'	},
		{0,		0,			0,	0	}
	};

	// process options
	while((opt = getopt_long(argc, argv, "dk:f:o:h", long_options, &option_index)) != -1)
	{
		switch(opt)
		{
			case 'k':
				// assign keyword
				keyword = optarg;
				curarg++; // too many args?
				break;
			case 'h':
				// help
				printHelp();
				return 0;
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
			default:
				break;
		}

		curarg++; // too many args?
	}

	if(!keyword)
	{
		printf("Haha you suck!\n");

		return 0;
	}

	char* toXcrypt;
	if(file_to_read == NULL)
	{
		toXcrypt = argv[curarg++];

		if(curarg != argc)
		{
			printf("LOL nope!\n");
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
