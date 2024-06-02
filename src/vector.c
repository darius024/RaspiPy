#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "vector.h"

#define	GROW_FACTOR 2

vector *initializeVector(size_t maxSize, size_t elementSize) {
	vector *v = (vector *)malloc(sizeof(vector));
	if (v == NULL) {
		fprintf(stderr, "Coudln't malloc\n");
		exit(EXIT_FAILURE);
	}
	v -> data = malloc(maxSize * elementSize);
	v -> currentSize = 0;
	v -> maxSize = maxSize;
	v -> elementSize = elementSize;
	return v;
}

// assume element is the right type to be added
void addToVector(vector *v, void *element) {
	//Check element has right type

	//Check if resize is required
	if (v -> currentSize == v -> maxSize) {
		v -> data = realloc(v -> data, (v -> maxSize) * (v -> elementSize) * GROW_FACTOR);
		v -> maxSize *= GROW_FACTOR;
	}
	void *target = (char *)(v -> data) + (v -> currentSize) * (v -> elementSize);
	memcpy(target, element, v -> elementSize);
	(v -> currentSize)++;
}

void freeVector(vector *v) {
	free(v -> data);
	free(v);
}

// Must cast result to type of element retrieving - otherwise derefencing void pointer
void *getFromVector(vector *v, size_t index) {
	if (index >= v -> currentSize) {
		fprintf(stderr, "Invalid index\n");
		exit(EXIT_FAILURE);
	}
	return (char *)(v -> data) + (v -> elementSize) * index;
}

