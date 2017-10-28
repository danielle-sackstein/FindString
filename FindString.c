#include <stdio.h>
#include <string.h>

int printLine(FILE *fp, int startOfLineOffset, char *fileName)
{
	fseek(fp, startOfLineOffset, SEEK_SET);

	if (fileName != 0) {
		printf("%s:", fileName);
	}

	int offset = startOfLineOffset;
	for (char c = fgetc(fp); (c != EOF) && (c != '\n'); c = fgetc(fp), offset++) {
		printf("%c", c);
	}

	printf("\n");

	return offset;
}

void findWordsInFile(char *wordToFind, char *fileName, char *printFileName)
{
	FILE *fp = fopen(fileName, "r");
	if (fp == 0) {
		return;
	}

	int wordLength = strlen(wordToFind);
	int letterCount = 0;
	int startOfLineOffset = 0;

	int offset = 0;

	for (char c = fgetc(fp);; c = fgetc(fp), offset++) {

		if (c == '\n' || c == EOF) {
			if (letterCount == wordLength) {
				offset = printLine(fp, startOfLineOffset, printFileName);
			}
			if (c == EOF) {
				break;
			}

			startOfLineOffset = offset + 1;
			letterCount = 0;
		}
		else if (c == '\r') {
		}
		else if (c == ' ') {
			if (letterCount == wordLength) {
				offset = printLine(fp, startOfLineOffset, printFileName);
				startOfLineOffset = offset + 1;
			}
			letterCount = 0;
		}
		else {
			if (letterCount == wordLength) {
				letterCount = -1;
			}
			else if ((letterCount != -1) && (c == wordToFind[letterCount])) {
				letterCount++;
			}
			else {
				letterCount = -1;
			}
		}
	}

	fclose(fp);
}

int main(int argc, char *argv[])
{
	char *wordToFind = argv[1];

	int fileCount = argc - 2;

	for (int i = 2; i < argc; i++) {

		char *fileName = argv[i];

		findWordsInFile(
				wordToFind,
				fileName,
				fileCount == 1 ? 0 : fileName);
	}

	return 0;
}