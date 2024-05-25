#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCOMPONENTS 4
#define SPACE " "

/*
movz x1,#1
add x2,x1,#2
and x0, x0, x0
*/

void parserRouter(char *instruction) {
	int numComponents;
	char ** components = decompose(instruction, &numComponents);
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

int main(int argc, char **argv) {
	if (argc >= 3) {
		printf("%s\n", argv[1]);
		printf("%s\n", argv[2]);
	}	
	else {
		fprintf(stderr, "More arguments needed\n");
	}
	int numComponents;
	char str[] = "hello there";
	char **a = decompose(str, &numComponents);
	printf("components: %d\n", numComponents);
	for (int i = 0 ; i < numComponents ; i++) {
		printf("%s\n", a[i]);
	}

	return EXIT_SUCCESS;
}

/*
void comps(char *instruction) {
	
	char *comp = strtok(instruction, SPACE);
	while (comp != NULL) {
		printf("%s\n", comp);
		comp = strtok(NULL, SPACE);
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
	char str[] = "hello there";
	comps(str);
	return 0;
}
*/





