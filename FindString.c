#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define OK                            0
#define FILE_OPEN_FAILED            (-1)
#define FILE_READ_FAILED			(-2)

#define MAX_CHARS_IN_LINE			(256)
#define MAX_BYTES_IN_LINE			(256 + 2 + 1) // allow \n or \r\n at the end of the line + null

/**
 * Prints a line
 * @param printFileName a pointer to the files name
 * @param line a pointer to the line to print
 */
void printLine(char *printFileName, char *line)
{
	if (printFileName != 0) {
		printf("%s:", printFileName);
	}
	printf("%s", line);
}

/**
 * Checks if the letters are equal (even if one of them is/isn't case sensitive.
 * @param left first character
 * @param right second character
 * @return true if they are equal, false otherwise.
 */
bool isCaseInsensitiveEqual(char left, char right)
{
	return toupper(left) == toupper(right) ? true : false;
}

/**
 * Seeks the line and checks if the word appears in it.
 * @param printFileName a pointer to the files name
 * @param wordToFind a pointer to the word
 * @param line a pointer to the line that is checked.
 */
void findWordsInLine(char *printFileName, char *wordToFind, char *line)
{
	/* As the source of line is the fgets function it is guaranteed that
	 * it contains a null somewhere in its allocated space.
	 */
	size_t lineLength = strlen(line);
	size_t wordLength = strlen(wordToFind);

	/* letterCount counts the number of consecutive letters from the line have been read
	 * that are the same as those in wordToFind.
	 * The maximum number of chars in wordToFind is 512 so letterCount will never
	 * become negative when counting the number of letters.
	 * Therefore we can give -1 a special meaning.
	 * If letterCount is -1, the current position cannot be the start of wordToFind.
	 */

	int letterCount = 0;

	for (size_t i = 0; i < lineLength; i++) {
		char c = line[i];
		if (c == ' ' || c == '\r' || c == '\n') {

			if (letterCount == (int) wordLength) {
				printLine(printFileName, line);
				return;
			}
			letterCount = 0;
		}
		else if (letterCount != -1) {
			if ((letterCount < (int) wordLength) &&
					isCaseInsensitiveEqual(c, wordToFind[letterCount])) {
				letterCount++;
			}
			else {
				letterCount = -1;
			}
		}
	}

	/* This handles the case that the wordToFind was at the end of the line and the line
	 * did not end in a newline (i.e. the last line in the file)  */

	if (letterCount == (int)wordLength) {
		printLine(printFileName, line);
	}
}

/**
 * Checks if a word exists in the file, and prints the lines which the word appears in,
 * @param wordToFind a word to look for
 * @param fileName a pointer to the files name
 * @param printFileName a pointer to the print files name
 * @return o if there were no errors, and 1 if there were
 */
int findWordsInFile(char *wordToFind, char *fileName, char* printFilename)
{
	FILE *fp = fopen(fileName, "r");
	if (fp == 0) {
		return FILE_OPEN_FAILED;
	}

	char line [MAX_BYTES_IN_LINE];

	while (true) {

		char* str = fgets(line, MAX_BYTES_IN_LINE, fp);
		if (str == 0) {
			if (feof(fp)) {
				/* The end of the file has been reached */
				break;
			}
			else{
				return FILE_READ_FAILED;
			}
		}
		findWordsInLine(printFilename, wordToFind, line);
	}

	/* line does not need to be freed in the loop, because as described above, it
	 * is reused if large enough and freed internally and replaced if not. */



	fclose(fp);

	return OK;
}

/**
 * The main method. Gets at least 3 arguments, and checks if a word exists in each file.
 * @param argc the number of arguments
 * @param argv a pointer to the arguments
 * @return o if there were no errors, and 1 if there were
 */
int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr,
            "Wrong number of arguments.\n"
            "You need to enter at least two arguments\n"
            "1. a word to search for.\n"
            "2. an existing file name.\n");
		return EXIT_FAILURE;
	}

	char *wordToFind = argv[1];
	bool shouldPrintFilename = argc > 3;

	for (int i = 2; i < argc; i++) {

		char *fileName = argv[i];
		int error = findWordsInFile(
				wordToFind,
				fileName,
				shouldPrintFilename ? fileName : 0);

		if (error != OK) {
			fprintf(stderr, "grep: %s: No such file or directory\n", fileName);
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}