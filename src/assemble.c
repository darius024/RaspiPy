#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemble.h"
#include "disassembler.h"
#include "utils.h"
#include "vector.h"

#define SPACE " "
#define SPACECOMMA ", "
#define OPCODE 0
#define BUFFER_LENGTH 128
#define MAX_TOKEN_LENGTH 20
#define CPOINTER_SIZE sizeof(char *)
#define MAX_INSTRS 200
// #define NOP_INSTRUCTION 0xd503201f

#define SIZE_DP sizeof(dataProcessing) / sizeof(char *)
#define SIZE_B sizeof(branching) / sizeof(char *)
#define SIZE_LS sizeof(loadAndStore) / sizeof(char *)
#define SIZE_DIR sizeof(directive) / sizeof(char *)

// int instr[MAX_INSTRS];
int PC;

typedef struct
{
    char *tokens[NUM_TOKENS];
    char instrname[MAX_TOKEN_LENGTH];
    char buff[BUFFER_LENGTH];
} InstructionParse;

FILE *outputFile;

// symtable stores symbolTables as elements
vector *symtable;

// undefLables stores labelMaps as elements
vector *undeftable;

enum type {
    dp,  // data processing
    ls,  // load/store
    b,   // branch
    dir, // .int
    lb   // label
};

// Global keywords to determine instruction type
static const char *dataProcessing[] = {
    "add", "adds", "sub", "subs",
    "cmp", "cmn",
    "neg", "negs",
    "and", "ands", "bic", "bics", "eor", "orr", "eon", "orn",
    "tst",
    "movk", "movn", "movz", "mov", "mvn",
    "madd", "msub",
    "mul", "mneg"};

static const char *branching[] = {
    "b", "br", "b.eq", "b.ne", "b.ge", "b.lt", "b.gt", "b.le", "b.al"};

static const char *loadAndStore[] = {
    "str", "ldr"};

static const char *directive[] = {
    ".int"};

InstructionParse *initializeStruct()
{
    InstructionParse *instr = (InstructionParse *)malloc(sizeof(InstructionParse));
    // Allocate memory for tokens
    for (int i = 0; i < NUM_TOKENS; i++)
    {
        instr->tokens[i] = (char *)malloc((MAX_TOKEN_LENGTH + 1) * sizeof(char));
        if (instr->tokens[i] == NULL)
        {
            perror("Failed to allocate space for tokens.\n");
            for (int j = 0; j < i; j++) {
                free(instr->tokens[j]);
            }
            exit(EXIT_FAILURE);
        }
    }
    return instr;
}

void freeStruct(InstructionParse *instr)
{
    for (int i = 0; i < NUM_TOKENS; i++) {
        free(instr->tokens[i]);
    }
    free(instr);
}

static Instruction *initializeInstruction() 
{
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));

    if (instruction == NULL) {
        perror("Failed to allocate space for Instruction struct.\n");
        exit(EXIT_FAILURE);
    }
    return instruction;
}

static void freeInstruction(Instruction *instruction) 
{
    free(instruction);
}

void updateSymbolTable(InstructionParse *instr)
{
    char *p = strchr(instr->instrname, ':');
    if (p != NULL)
    {
        // Label Case - Update the symbol table
        *p = '\0';
		struct symbolTable *symtableEntry = (struct symbolTable *)malloc(sizeof(struct symbolTable));
		strcpy(symtableEntry->label, instr->instrname);
		symtableEntry->address = PC * 4;
		addToVector(symtable, symtableEntry);
	}
}

void decompose(InstructionParse *instr, int *numTokens)
{                                             
    char *instrSavePntr = NULL; // Save pointer for instruction

    // Break down an instruction into its tokens
    char *token = strtok_r(instr->buff, SPACE, &instrSavePntr); // ignores any indent at start
    // Take the mnemonic of the instruction
    strcpy(instr->instrname, token);
    // Take the first token
    token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    *numTokens = 0;
    while (token != NULL) {
        strcpy(instr->tokens[*numTokens++], token);
        token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    }
}

enum type identifyType(char *instrname)
{
    // Label
    if (strchr(instrname, ':') != NULL) {
        return lb;
    }
    // Directive
    if (checkWordInArray(instrname, directive, SIZE_DIR)) {
        return dir;
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

void disassemble(Instruction *instruction, InstructionParse *instr)
{
    // Sends instruction to corresponding disassembler
    int numTokens;
    decompose(instr, &numTokens);
    enum type instrType = identifyType(instr->instrname);

    switch (instrType)
    {
    case lb:
        updateSymbolTable(instr);
        return; // no update of PC
    case dir:
        disassembleDir(instr->tokens[0], outputFile);
        break;
    case b:
        disassembleB(instr->instrname, instr->tokens[0], instruction, outputFile, PC);
        break;
    case ls:
        disassembleSDT(instr->instrname, instr->tokens, numTokens, instruction, outputFile, PC);
        break;
    case dp:
        if (getOpType(instr->tokens) == imm) {
            disassembleDPI(instr->instrname, instr->tokens, numTokens, instruction);
        } else {
            disassembleDPR(instr->instrname, instr->tokens, numTokens, instruction);
        }
        break;
    }
    // Update PC
    PC++;
}

void onePass(FILE *file, InstructionParse *instr)
{
    PC = 0;
    while (fgets(instr->buff, BUFFER_LENGTH, file))
    {
        // Remove empty line
        instr->buff[strlen(instr->buff) - 1] = '\0';
        // Check that buff is not empty
        if (*instr->buff != '\0') {
            disassembleRouter(instr);
        }
    }
}

int main(int argc, char **argv)
{	
    char *inputFilename;
    char *outputFilename;
    if (argc >= 3) {
        inputFilename = argv[1];
        outputFilename = argv[2];
    } else {
        perror("Provide both an input and an output file.\n");
        exit(EXIT_FAILURE);
    }

	// Initializing data types
    InstructionParse *instr = initializeStruct();
    Instruction *instruction = initializeInstruction();
	symtable = initializeVector(MAX_INSTRS, sizeof(struct symbolTable));
	undeftable = initializeVector(MAX_INSTRS, sizeof(struct undefTable));

    // Prepare output file for writing
    openBinaryFile(outputFilename);

    // Read from assembly file and disassemble each line
    loadAssemblyFile(inputFilename, instr);

    // One-pass: Compute previous undefined instructions
    handleUndefTable(outputFile);

    // Freeing data types
    freeStruct(instr);
    freeInstruction(instruction);
	freeVector(symtable);
   	freeVector(undeftable);

    fclose(outputFile);

    return EXIT_SUCCESS;
}
