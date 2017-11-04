#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define OK                            0
#define FILE_OPEN_FAILED            (-1)

#define WRONG_NUMBER_OF_ARGUMENTS   "Wrong number of arguments." \
                                    " You need to enter at least two" \
                                    " arguments 1. a word to search for." \
                                    " 2.an existing file name. \n" \

#define FILE_OPEN_ERROR            ": No such file or directory.\n" \


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

	size_t wordLength = strlen(wordToFind);
	size_t lineLength = strlen(line);

	/* letterCount counts the number of letters from the line have been read
	 * that are the same as those in wordToFind.
	 * If its value is -1, the current position cannot be the start of wordToFind.
	 */

	int letterCount = 0;

	for (size_t i = 0; i < lineLength; i++) {
		char c = line[i];
		if (c == ' ' || c == '\r' || c == '\n') {

			/* We recognize the word only if the next character is a space, a new line
			 * or the end of the line. The end of the line is detected after the loop */

			if (letterCount == (int) wordLength) {
				printLine(printFileName, line);
				return;
			}

			/* After having seen a space or newline, we can restart the search for wordToFind */
			letterCount = 0;
		}
		else if (letterCount != -1) {
			if (letterCount == (int) wordLength) {
				/* If we have already seen all letters of wordToFind
				 * then that word does not count because this character is not a space
				 * Moreover, we cannot start a new search yet because the previous letter
				 * was not a space, so reset letterCount to -1 */
				letterCount = -1;
			}
			else if (isCaseInsensitiveEqual(c, wordToFind[letterCount])) {
				/* We are on track to find the word, look for the next letter */
				letterCount++;
			}
			else {
				/* The match has failed.*/
				letterCount = -1;
			}
		}
	}

	/* This handles the case that the wordToFind was at the end of the line and the line
	 * did not end in a newline (the last line in the file)  */

	if (letterCount == (int) wordLength) {
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
int findWordsInFile(char *wordToFind, char *fileName, char *printFileName)
{

	FILE *fp = fopen(fileName, "r");
	if (fp == 0) {
		return FILE_OPEN_FAILED;
	}

	char *line = 0;
	size_t size = 0;

	while (true) {
		/* see explanation for getline at http://man7.org/linux/man-pages/man3/getline.3.html
		 * In the first call, line will be allocated by getline and the size of the allocation
		 * is placed in size.
		 * On every following call, we pass in the same allocated pointer. If it is large enough
		 * it will be reused. If not it will be freed and replaced by one that is large enough.
		*/
		ssize_t lineLength = getline(&line, &size, fp);
		if (lineLength == -1) {
			/* The end of the file has been reached */
			break;
		}
		findWordsInLine(printFileName, wordToFind, line);
	}

	/* line does not need to be freed in the loop, because as described above, it
	 * is reused if large enough and freed internally and replaced if not. */

	if (line != 0) {
		free(line);
	}
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
		fprintf(stderr, WRONG_NUMBER_OF_ARGUMENTS);
		return EXIT_FAILURE;
	}

	char *wordToFind = argv[1];
	int fileCount = argc - 2;

	for (int i = 2; i < argc; i++) {

		char *fileName = argv[i];
		int error = findWordsInFile(
				wordToFind,
				fileName,
				fileCount == 1 ? 0 : fileName);
		if (error != OK) {
			//TODO: print the file name before
			fprintf(stderr, FILE_OPEN_ERROR);
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}