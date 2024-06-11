// Vector data strcuture for the symbol tables

#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>


// Vector Structure
typedef struct {
	void *data;
	size_t currentSize;
	size_t maxSize;
	size_t elementSize;
} vector;

// Prototypes
extern vector *initializeVector(size_t maxSize, size_t elementSize);
extern void freeVector(vector *v);
extern void addToVector(vector *v, void *element);
extern void *getFromVector(vector *v, size_t index);

#endif
