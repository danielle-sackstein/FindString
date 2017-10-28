#include <stdio.h>
#include <string.h>

int printLine(FILE *fp, int startOfLineOffset)
{
	fseek(fp, SEEK_SET, startOfLineOffset);

	int offset = startOfLineOffset;
	for (char c = fgetc(fp); (c != EOF) && (c != '\n'); c = fgetc(fp), offset++) {
		printf("%c", c);
	}

	return offset;
}

void findWordsInFile(char *wordToFind, char *fileName)
{
	FILE *fp = fopen(fileName, "r");
	if (fp == 0) {
		return;
	}

	int wordLength = strlen(wordToFind);
	int letterCount = 0;
	int startOfLineOffset = 0;

	int offset = 0;

	for (char c = fgetc(fp); c != EOF; c = fgetc(fp), offset++) {
		if (c == '\n') {
			if (letterCount == wordLength) {
				offset = printLine(fp, startOfLineOffset);
			}
			startOfLineOffset = offset + 1;
			letterCount = 0;
		}
		else if (c == ' ') {
			if (letterCount == wordLength) {
				offset = printLine(fp, startOfLineOffset);
				startOfLineOffset = offset + 1;
			}
			letterCount = 0;
		}
		else {
			if (letterCount == wordLength) {

				offset = offset - letterCount;
				fseek(fp, SEEK_SET, offset + 1);
				letterCount = 0;
			}
			else if (c == wordToFind[letterCount]) {
				letterCount++;
			}
			else {
				letterCount = 0;
			}
		}
	}

	fclose(fp);
}

int main(int argc, char *argv[])
{
	char *wordToFind = argv[1];

	for (int i = 2; i < argc; i++) {
		findWordsInFile(wordToFind, argv[i]);
	}

	printf("Hello, World!\n");
	return 0;
}