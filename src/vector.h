#include <stdio.h>

#ifndef VECTOR
#define VECTOR

typedef struct {
	void *data;
	size_t currentSize;
	size_t maxSize;
	size_t elementSize;
} vector;

extern vector *initializeVector(size_t maxSize, size_t elementSize);
extern void freeVector(vector *v);
extern void addToVector(vector *v, void *element);
extern void *getFromVector(vector *v, size_t index);

#endif
