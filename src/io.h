#include <stdio.h>

#define STDOUT "stdout"

// Function pointer
typedef void (*function)(FILE *);

// Prototypes
extern FILE *loadInputFile(const char *filename, const char *extension, const char *readMode);
extern FILE *openOutputFile(const char *filename, const char *extension, const char *writeMode);
extern void checkError(bool error);
extern void checkErrorOutput(FILE *file);
extern void closeFiles(FILE *input, FILE *output);
