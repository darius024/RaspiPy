#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc >= 3) {
		printf("%s\n", argv[1]);
		printf("%s\n", argv[2]);
	}	
	else {
		fprintf(stderr, "More arguments needed\n");
	}
	return EXIT_SUCCESS;
}
