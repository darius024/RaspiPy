#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"


// IO Handling
FILE *loadInputFile(const char *filename, const char *extension, const char *readMode)
{
    if (extension != NULL) { // Check if a specific extension is required
        const char *fileType = strrchr(filename, '.');
        if (!fileType || strcmp(fileType + 1, extension) != 0) {
            fprintf(stderr, "This is not a valid .%s file: %s\n", extension, filename);
            exit(EXIT_FAILURE);
        }
    }

    FILE *file;
    file = fopen(filename, readMode);
    if (!file) {
        perror("Could not open input file.");
        exit(EXIT_FAILURE);
    }
    return file;
}

FILE *openOutputFile(const char *filename, const char *extension, const char *writeMode)
{
    if (extension != NULL) { // Check if a specific extension is required
        const char *fileType = strrchr(filename, '.');
        if (strcmp(filename, STDOUT) != 0 && (!fileType || strcmp(fileType + 1, extension) != 0)) {
            fprintf(stderr, "This is not a valid .%s file: %s\n", extension, filename);
            exit(EXIT_FAILURE);
        }
    }
    
    FILE *file;
    file = (!strcmp(filename, STDOUT)) ? stdout : fopen(filename, writeMode);
    if (!file) {
        perror("Could not open output file.\n");
        exit(EXIT_FAILURE);
    }
    return file;
}

// Error Checking
void checkError(bool error)
{
    if (error) {
        exit(EXIT_FAILURE);
    }
}

void checkErrorOutput(FILE *file)
{
    if (ferror(file)) {
        perror("Error ocurred writing to the output.\n");
        exit(EXIT_FAILURE);
    }
}

// File Close
void closeFiles(FILE *input, FILE *output)
{
    fclose(input);
    checkErrorOutput(output);
    fclose(output);
}
