#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCOMPONENTS 4
#define SPACE " "
#define OPCODE 0

/*
movz x1,#1
add x2,x1,#2
and x0, x0, x0
*/

extern char **decompose(char *instruction, int *numComponents);
extern void parserRouter(char *instruction);

extern void branchParser(char **components, int numComponents);
extern void loadStoreParser(char **components, int numComponents);
extern void specialParser(char **components, int numComponents);
extern void dpParser(char **components, int numComponents);

void branchParser(char **components, int numComponents){
	return;
}

void loadStoreParser(char **components, int numComponents){
	return;
}

void specialParser(char **components, int numComponents){
	return;
}

void dpParser(char **components, int numComponents){
	return;
}

char **decompose(char *instruction, int *numComponents) { // returns array of each component (string) of an instruction
	char **components = malloc(MAXCOMPONENTS * sizeof(char *));	// assumes instruction is non-empty (not empty line)
	char *comp = strtok(instruction, SPACE);
	int count = 0;

	if (components == NULL) { // Added check for memory allocation success
       		fprintf(stderr, "Memory allocation failed\n");
       	 	exit(EXIT_FAILURE);
    	}

	while (comp != NULL && count < MAXCOMPONENTS) {
		components[count] = comp;
		count++;
		printf("%s\n", comp);
		comp = strtok(NULL, SPACE);
	}
	*numComponents = count;
	return components;

}

void parserRouter(char *instruction) {
	if (*instruction == '\n') {
		return;
	}
	int numComponents;
	char **components = decompose(instruction, &numComponents);
	char *opcode = components[0];
	if (strcmp(opcode, "b") == 0 || strcmp(opcode, "b.cond") || strcmp(opcode, "br") == 0) {
		branchParser(components, numComponents);
	} else if (strcmp(opcode, "str") == 0 || strcmp(opcode, "ldr") == 0) {
		loadStoreParser(components, numComponents);
	} else if (strcmp(opcode, ".int") == 0) {
		specialParser(components, numComponents);
	} else {
		dpParser(components, numComponents);
	}
}

int main(int argc, char **argv) {
	if (argc >= 3) {
		printf("%s\n", argv[1]);
		printf("%s\n", argv[2]);
	}	
	else {
		fprintf(stderr, "More arguments needed\n");
	}	
	// while loop here to fetch each line of assembly code. Each line is then fed into routeParser
	
	return EXIT_SUCCESS;
}





