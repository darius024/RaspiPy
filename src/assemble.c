#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatypes_as.h"
#include "decoders.h"
#include "disassembler.h"
#include "io.h"
#include "onepass.h"
#include "structs.h"
#include "utils_as.h"
#include "vector.h"

#define SPACE " "
#define SPACECOMMA ", "

#define END_OF_FILE 0
#define IN_FILE 1

extern uint32_t binaryInstr;
// Keep track of address of instruction executed
extern int PC;

// symtable stores symbolTables as elements
vector *symtable;

// undefLables stores labelMaps as elements
vector *undeftable;

//
// Update Data Structures
//
void updateBinaryInstr(uint32_t instruction)
{
    binaryInstr[PC++] = instruction;
}

void updateSymbolTable(InstructionParse *instr)
{
    char *p = strchr(instr->instrname, ':');
    if (p != NULL) { // Label case
        *p = '\0';
		struct symbolTable *symtableEntry = (struct symbolTable *)malloc(sizeof(struct symbolTable));
		strcpy(symtableEntry->label, instr->instrname);
		symtableEntry->address = PC * 4;
		addToVector(symtable, symtableEntry);
	}
}

// Retrieve type of instruction being parsed
enum type identifyType(char *instrname)
{
    // Label
    if (strchr(instrname, LABEL_ID) != NULL) {
        return lb;
    }
    // Directive
    if (checkWordInArray(instrname, directive, SIZE_DIR)) {
        return dir;
    }
    // Alias
    if (checkWordInArray(instrname, aliases, SIZE_ALS)) {
        return als;
    }
    // Branching
    if (checkWordInArray(instrname, branching, SIZE_B)) {
        return b;
    }
    // Load and Stores
    if (checkWordInArray(instrname, loadAndStore, SIZE_LS)) {
        return ls;
    }
    // Data Processing
    if (checkWordInArray(instrname, dataProcessing, SIZE_DP)) {
        return dp;
    }
    perror("Unsupported instruction name (mnemonic).\n");
    exit(EXIT_FAILURE);
}

// Decompose an instruction into its correspoding type and tokens
int decompose(InstructionParse *instr, FILE *input)
{                     
    if (!fgets(instr->buff, BUFFER_LENGTH, input)) {
        return END_OF_FILE;
    }
    instr->buff[strlen(instr->buff) - 1] = '\0';

    char *instrSavePntr = NULL; // Save pointer for instruction

    // Break down an instruction into its tokens
    char *token = strtok_r(instr->buff, SPACE, &instrSavePntr); // ignores any indent at start
    // Take the mnemonic of the instruction
    strcpy(instr->instrname, token);
    instr->type = identifyType(instr->instrname);

    // Take the first token
    token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    instr->numTokens = 0;
    while (token != NULL) {
        strcpy(instr->tokens[instr->numTokens++], token);
        token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    }
    return IN_FILE;
}

// Construct a specific data structure out of the tokens based on instruction type
int disassemble(InstructionParse *instr, Instruction *instruction, bool *disassembled)
{
    if (*(instr->buff) == '\0') {
        return EXIT_SUCCESS;
    }
    // Sends instruction to corresponding disassembler
    *disassembled = false;
    switch (instr->type) {
        case lb:
            updateSymbolTable(instr);
            return;
        case dir:
            updateBinaryInstr(getInt(instr->tokens[0]));
            return;
        case als:
            disassembleAlias(instr, instruction);
            disassemble(instr, instruction, disassembled);
            break;
        case b:
            disassembleB(instr, instruction);
            break;
        case ls:
            disassembleSDT(instr, instruction);
            break;
        case dp:
            if (getOpType(instr->tokens) == imm) {
                disassembleDPI(instr, instruction);
            } else {
                disassembleDPR(instr, instruction);
            }
            break;
    }
    *disassembled = true;
    return EXIT_SUCCESS;
}

//
// IO Handling
//
void writeBinaryInstr(FILE *file)
{
    fwrite(binaryInstr, sizeof(int), PC, file);
}

//
// Main Program
//
int main(int argc, char **argv)
{	
    char *inputFile;
    char *outputFile;
    if (argc >= 2) {
        inputFile = argv[1];
        outputFile = (argc > 2) ? argv[2] : STDOUT;
    } else {
        perror("Provide at least an input file.\n");
        exit(EXIT_FAILURE);
    }

	// Initializing data types
    uint32_t *instr = &binaryInstr[PC];
    Instruction *instruction = initializeInstruction();
    InstructionParse *instructionParse = initializeInstructionParse();
	symtable = initializeVector(MAX_INSTRS, sizeof(struct symbolTable));
	undeftable = initializeVector(MAX_INSTRS, sizeof(struct undefTable));
    bool disassembled;

    // Read from assembly file and disassemble each line
    FILE *input = loadInputFile(inputFile, "s", "r");
    // Prepare output file for writing
    FILE *output = openOutputFile(outputFile, "bin", "wb");

    while(decompose(instructionParse, input) != END_OF_FILE) {
        int disassembleError = disassemble(instructionParse, instruction, &disassembled);
        checkError(disassembleError);
        int decodeError = (disassembled) ? decode(instr, instruction, putBits) : EXIT_SUCCESS;
        checkError(decodeError);
    }

    // One-pass: Compute previous undefined instructions
    handleUndefTable(outputFile);

    // Freeing data types
    freeInstructionParse(instructionParse);
    freeInstruction(instruction);
	freeVector(symtable);
   	freeVector(undeftable);

    // Write the binary instructions
    writeBinaryInstr(output);

    // Close files
    closeFiles(input, output);

    return EXIT_SUCCESS;
}
