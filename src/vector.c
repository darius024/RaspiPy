#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

#define	GROW_FACTOR 2

vector *initializeVector(size_t maxSize, size_t elementSize) {
	vector *v = (vector *)malloc(sizeof(vector));
	if (v == NULL) {
		perror("Couldn't malloc vector.\n");
		exit(EXIT_FAILURE);
	}
	v->data = malloc(maxSize * elementSize);
	v->currentSize = 0;
	v->maxSize = maxSize;
	v->elementSize = elementSize;
	return v;
}

void freeVector(vector *v) {
	free(v->data);
	free(v);
}

// Assume element is the right type to be added
void addToVector(vector *v, void *element) {
	// Check element has compatible type
	assert(sizeof(*element) == v->elementSize);

	// Check if resizing is required
	if (v->currentSize == v->maxSize) {
		v->data = realloc(v->data, (v->maxSize) * (v->elementSize) * GROW_FACTOR);
		v->maxSize *= GROW_FACTOR;
	}
	void *target = (char *)(v->data) + (v->currentSize) * (v->elementSize);
	memcpy(target, element, v->elementSize);
	(v->currentSize)++;
}

// Must cast result to the type of element retrieving - otherwise derefencing void pointer
void *getFromVector(vector *v, size_t index) {
	if (index >= v->currentSize || index < 0) {
		fprintf(stderr, "Index out of bounds in vector.\n");
		exit(EXIT_FAILURE);
	}
	return (char *)(v->data) + (v->elementSize) * index;
}

