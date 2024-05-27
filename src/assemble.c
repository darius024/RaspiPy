#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCOMPONENTS 4
#define SPACE " "
#define SPACECOMMA ", "
#define OPCODE 0
#define BUFFER_LENGTH 128
#define MAX_LABEL_LENGTH 20
#define CPOINTER_SIZE sizeof(char *)
#define MAX_INSTRS 200


#define DP_SIZE sizeof(dataProcessing)/CPOINTER_SIZE 
#define SINGLE_OPERAND_SIZE sizeof(singleOperand)/CPOINTER_SIZE
#define TWO_OPERAND_NO_DEST_SIZE sizeof(twoOperandNoDest)/CPOINTER_SIZE
#define B_SIZE sizeof(branching)/CPOINTER_SIZE
#define LS_SIZE sizeof(loadAndStore)/CPOINTER_SIZE
#define DIR_SIZE sizeof(dir)/CPOINTER_SIZE
/*
movz x1,#1
add x2,x1,#2
and x0, x0, x0
*/

int numLabel = 0;

int instr[MAX_INSTRS];
int PC;

struct symbolTable {
	char label[MAX_LABEL_LENGTH];
	int address;
} symtable[100];

enum type {
	dp,
	ls,
	b,
	dir, // .int
	lb
};
	
// Defined here so that code compiles
void dpDisassemble(char **tokens, int numInstructions) {
	return;
}
void lsDisassemble(char **tokens, int numInstructions) {
	return;
}
void bDisassemble(char **tokens, int numInstructions) {
	return;
}
void dirDisassemble(char **tokens, int numInstructions) {
	return;
}


const char *dataProcessing[] = {
"add", "adds", "sub", "subs",
"cmp", "cmn",
"neg", "negs",
"and", "ands", "bic", "bics", "eor", "orr", "eon", "orn",
"tst",
"movk", "movn", "movz", "mov", "mvn",
"madd", "msub",
"mul", "mneg"
};

const char *singleOperand[] = {
"mov", "mul", "mneg", "neg", "negs", "mvn", "movz", "movn", "movk"
};

const char *twoOperandNoDest[] = {
"cmp", "cmn", "tst"
};

const char *branching[] = {
"b", "br", "beq", "bne", "bge", "blt", "bgt", "ble", "bal"
};

const char *loadAndStore[] = {
"str", "ldr"
};

const char *directive[] = {
".int"
};



// use strtok_r instead of strtok
char **decompose(char *instruction, int *numTokens) { // returns array of each token (string) of an instruction
	char **tokens = malloc(MAXCOMPONENTS * sizeof(char *));
	char *comp = strtok(instruction, SPACE); // ignores any indent at start
	int count = 0;

	if (tokens == NULL) { // Added check for memory allocation success
       		fprintf(stderr, "Memory allocation failed\n");
       	 	exit(EXIT_FAILURE);
    	}

	while (comp != NULL && count < MAXCOMPONENTS) {
		tokens[count] = comp;
		count++;
		printf("%s\n", comp);
		comp = strtok(NULL, SPACECOMMA);
	}
	*numTokens = count;
	return tokens;

}

enum type identifyType(char *instrname) {
	char *p = strchr(instrname, ':');
	if (p) {
		return lb;
	}
	for (int i = 0 ; i < DIR_SIZE ; i++) {
		if (strcmp(instrname, directive[i]) == 0) {
			return dir;
		}
	}	
	for (int i = 0 ; i < B_SIZE ; i++) {
		if (strcmp(instrname, branching[i]) == 0) {
			return b;
		}
	}
	for (int i = 0 ; i < LS_SIZE ; i++) {
		if (strcmp(instrname, loadAndStore[i]) == 0) {
			return ls;
		}
	}
	for (int i = 0 ; i < DP_SIZE ; i++) {
		if (strcmp(instrname, dataProcessing[i]) == 0) {
			return dp;
		}
	}

	fprintf(stderr, "unidentifiable type of line");
	exit(EXIT_FAILURE);
}

void disassembleRouter(char *instruction) { // Sends instruction to corresponding disassembler e.g. branchDisassemble
	if (*instruction == '\n') {      
		return;
	}
	int numTokens;
	char **tokens = decompose(instruction, &numTokens);
	char *instrname = tokens[0];
	enum type instrType = identifyType(instrname);

	switch(instrType) {
		case lb:
			break; // Nothing to parse
		case dir:
			dirDisassemble(tokens, numTokens); // free the memory occupied by the tokens in these functions
		case b:
			bDisassemble(tokens, numTokens);
		case ls:
			lsDisassemble(tokens, numTokens);
			break;
		case dp:
			dpDisassemble(tokens, numTokens);
			break;
		default:
			// Shouldn't reach here
			break;
	}	
}

void updateSymbolTable(char *buff) {
	char *p = strchr(buff, ':');
	if (p != NULL) { // assume only one : in a line, and must be label
		*p = '\0';
		strcpy(symtable[numLabel].label, buff);
		symtable[numLabel++].address = PC;
	}
	else {
		PC++;
	}
}

void loadAssemblyFile(char *filename) {
	const char *extension = strrchr(filename, '.');
	if (!extension || strcmp(extension + 1, "s") != 0) {
		fprintf(stderr, "This is not a valid assembly source file: %s\n", filename);
		exit(EXIT_FAILURE);
	}
	FILE *file;
	char buff[BUFFER_LENGTH];
	
	file = fopen(filename, "r");

	if (!file) {
		perror("Could not open input file.\n");
		exit(EXIT_FAILURE);
	}

	//Two pass approach

	//First pass - build symbol table
	PC = 0;
	while (fgets(buff, BUFFER_LENGTH, file)) {
		if (strlen(buff) > 1) {
			updateSymbolTable(buff);
		}
	}

	//Reset pointer to beginning for reread
	rewind(file);

	//Second pass - disassemble instructions
	PC = 0;
	while (fgets(buff, BUFFER_LENGTH, file)) {
		if (strlen(buff) > 1) {
			disassembleRouter(buff);
		}
	}

	fclose(file);
}

void writeToBinaryFile(const char *filename) {
	FILE *file = (strcmp(filename, "stdout")) ? stdout : fopen(filename, "wb");
	if (file == NULL) {
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}
	const char *extension = strrchr(filename, '.');
	if ((file != stdout) && (!extension || strcmp(extension, ".bin") != 0)) {
		fprintf(stderr, "Not a valid binary file: %s\n", filename);
		exit(EXIT_FAILURE);
	}
	for (int i = 0 ; i < PC ; i++) {
		fwrite(&instr[i], sizeof(int), 1, file);
	}
	
	if (file != stdout) {
		fclose(file);
	}
}

int main(int argc, char **argv) {
	char *inputFile;
	char *outputFile;
	if (argc >= 3) {
		inputFile = argv[1];
		outputFile = argv[2];
	}	
	else {
		fprintf(stderr, "More arguments needed\n");
		return EXIT_FAILURE;
	}	

	loadAssemblyFile(inputFile);

	writeToBinaryFile(outputFile);

	return EXIT_SUCCESS;
}

