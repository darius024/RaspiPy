#include <stdio.h>

// Function pointer
typedef void (*function)(FILE *);

// Prototypes
extern void loadInputFile(const char *filename, const char *extension, const char *readMode, function inputFunction);
extern void writeOutputFile(const char *filename, const char *extension, const char *writeMode, function outputFunction);
