#include <stdio.h>
#include <stdlib.h>

#include "io.h"

#define STDOUT "stdout"

// IO Handling

void loadInputFile(const char *filename, const char *extension, const char *readMode, function inputFunction) {
    const char *fileType = strrchr(filename, '.');
    if (!fileType || strcmp(fileType + 1, extension) != 0) {
        fprintf(stderr, "This is not a valid .%s file: %s\n", extension, filename);
        exit(EXIT_FAILURE);
    }

    FILE *file;
    file = fopen(filename, readMode);
    if (!file) {
        perror("Could not open input file.");
        exit(EXIT_FAILURE);
    }

    inputFunction(file);

    fclose(file);
}

void writeOutputFile(const char *filename, const char *extension, const char *writeMode, function outputFunction) {
    const char *fileType = strrchr(filename, '.');
    if (strcmp(filename, STDOUT) != 0 && (!fileType || strcmp(fileType + 1, extension) != 0)) {
        fprintf(stderr, "This is not a valid .%s file: %s\n", extension, filename);
        exit(EXIT_FAILURE);
    }
    
    FILE *file;
    file = (!strcmp(filename, STDOUT)) ? stdout : fopen(filename, writeMode);
    if (!file) {
        perror("Could not open output file.\n");
        exit(EXIT_FAILURE);
    }

    // Perform actions with the input file
    outputFunction(file);

    if (ferror(file)) {
        perror("Error ocurred writing to the output.\n");
        exit(EXIT_FAILURE);
    }

    if (file != stdout) {
        fclose(file);
    }
}
