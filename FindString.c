#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define OK                0
#define FILE_OPEN_FAILED (-1)

void printLine(char *printFileName, char *line) {
    if (printFileName != 0) {
        printf("%s:", printFileName);
    }
    printf("%s", line);
}

bool isCaseInsensitiveEqual(char left, char right) {
    return toupper(left) == toupper(right) ? true : false;
}

void findWordsInLine(char *printFileName, char *wordToFind, char *line) {

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

            if (letterCount == wordLength) {
                printLine(printFileName, line);
                return;
            }

            /* After having seen a space or newline, we can restart the search for wordToFind */
            letterCount = 0;
        } else if (letterCount != -1) {
            if (letterCount == wordLength) {
                /* If we have already seen all letters of wordToFind
                 * then that word does not count because this character is not a space
                 * Moreover, we cannot start a new search yet because the previous letter
                 * was not a space, so reset letterCount to -1 */
                letterCount = -1;
            } else if (isCaseInsensitiveEqual(c, wordToFind[letterCount])) {
                /* We are on track to find the word, look for the next letter */
                letterCount++;
            } else {
                /* The match has failed.*/
                letterCount = -1;
            }
        }
    }

    /* This handles the case that the wordToFind was at the end of the line and the line
     * did not end in a newline (the last line in the file)  */

    if (letterCount == wordLength) {
        printLine(printFileName, line);
    }
}

int findWordsInFile(char *wordToFind, char *fileName, char *printFileName) {

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

int main(int argc, char *argv[]) {
    char *wordToFind = argv[1];

    int fileCount = argc - 2;

    for (int i = 2; i < argc; i++) {

        char *fileName = argv[i];

        int error = findWordsInFile(
                wordToFind,
                fileName,
                fileCount == 1 ? 0 : fileName);
        if (error != OK) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}