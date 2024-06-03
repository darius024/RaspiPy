#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemble.h"
#include "disassembler.h"
#include "utils.h"
#include "vector.h"

#define NUM_TOKENS 5
#define SPACE " "
#define SPACECOMMA ", "
#define OPCODE 0
#define BUFFER_LENGTH 128
#define MAX_TOKEN_LENGTH 20
#define CPOINTER_SIZE sizeof(char *)
#define MAX_INSTRS 200
#define NOP_INSTRUCTION 0xd503201f

#define DP_SIZE sizeof(dataProcessing) / CPOINTER_SIZE
#define SINGLE_OPERAND_SIZE sizeof(singleOperand) / CPOINTER_SIZE
#define TWO_OPERAND_NO_DEST_SIZE sizeof(twoOperandNoDest) / CPOINTER_SIZE
#define B_SIZE sizeof(branching) / CPOINTER_SIZE
#define LS_SIZE sizeof(loadAndStore) / CPOINTER_SIZE
#define DIR_SIZE sizeof(directive) / CPOINTER_SIZE

// int instr[MAX_INSTRS];
int PC;

typedef struct
{
    char *tokens[NUM_TOKENS];
    char instrname[MAX_TOKEN_LENGTH];
    char buff[BUFFER_LENGTH];
} instruction;

FILE *outputFile;

//struct symbolTable symtable[MAX_INSTRS];
// symtable stores symbolTables as elements
vector *symtable;

//struct labelMap unDefLables[MAX_INSTRS];
// unDefLables stores labelMaps as elements
vector *unDefLables;

enum type
{
    dp,  // data processing
    ls,  // load/store
    b,   // branch
    nop, // nop
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

static const char *nopOp = "nop";
static uint32_t nopInstr = NOP_INSTRUCTION;

instruction *initializeStruct()
{
    instruction *instr = (instruction *)malloc(sizeof(instruction));

    // Allocate memory for tokens
    for (int i = 0; i < NUM_TOKENS; i++)
    {
        instr->tokens[i] = (char *)malloc((MAX_TOKEN_LENGTH + 1) * sizeof(char));
        if (instr->tokens[i] == NULL)
        {
            perror("Failed to allocate memory for tokens.\n");
            for (int j = 0; j < i; j++)
                free(instr->tokens[j]);
            exit(EXIT_FAILURE);
        }
    }

    return instr;
}

void freeStruct(instruction *instr)
{
    for (int i = 0; i < NUM_TOKENS; i++)
    {
        free(instr->tokens[i]);
    }
    free(instr);
}

void updateSymbolTable(instruction *instr)
{
    char *p = strchr(instr->instrname, ':');
    if (p != NULL)
    {
        // Label Case - Update the symbol table, no update of PC
        *p = '\0';
		int address = PC * 4;
		struct symbolTable *symtableEntry = (struct symbolTable *)malloc(sizeof(struct symbolTable));
		strcpy(symtableEntry -> label, instr -> instrname);
		symtableEntry -> address = address;
		addToVector(symtable, symtableEntry);
		// struct symbolTable *symtableEntry = (struct symbolTable *)getFromVector(symtable, numLabel++);
        //strcpy(symtable[numLabel].label, instr->instrname);
        // strcpy(symtableEntry -> label, instr->instrname);
		//symtable[numLabel++].address = PC * 4;
    	// symtableEntry -> address = PC * 4;
	}
}

// Use strtok_r instead of strtok
void decompose(instruction *instr, int *numTokens)
{                                             
    char *instrSavePntr = NULL; //save pointer for instruction maybe make global/static

    // splits an instruction into its tokens
    
    char *token = strtok_r(instr->buff, SPACE, &instrSavePntr); // ignores any indent at start
    // take the mnemonic of the instruction
    strcpy(instr->instrname, token);
    
    token = strtok_r(NULL, SPACECOMMA, &instrSavePntr); //get first arguement, guaranteed if well formed


    int count = 0;

    while (token != NULL)
    {
        strcpy(instr->tokens[count++], token);
        token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    }
    *numTokens = count;
}

enum type identifyType(char *instrname)
{
    char *p = strchr(instrname, ':');
    // Label
    if (p)
        return lb;
    // Directive
    for (int i = 0; i < DIR_SIZE; i++)
    {
        if (strcmp(instrname, directive[i]) == 0)
            return dir;
    }
    // Nop
    if (strcmp(instrname, nopOp) == 0)
        return nop;
    // Branching
    for (int i = 0; i < B_SIZE; i++)
    {
        if (strcmp(instrname, branching[i]) == 0)
            return b;
    }
    // Load and Stores
    for (int i = 0; i < LS_SIZE; i++)
    {
        if (strcmp(instrname, loadAndStore[i]) == 0)
            return ls;
    }
    // Data Processing
    for (int i = 0; i < DP_SIZE; i++)
    {
        if (strcmp(instrname, dataProcessing[i]) == 0)
            return dp;
    }

    fprintf(stderr, "Unidentifiable instruction name.");
    exit(EXIT_FAILURE);
}

void disassembleRouter(instruction *instr)
{
    // Sends instruction to corresponding disassembler
    int numTokens;
    // Construct the instrname and tokens array
    decompose(instr, &numTokens);
    enum type instrType = identifyType(instr->instrname);

    switch (instrType)
    {
    case lb:
        updateSymbolTable(instr);
        return; // No update of PC
    case dir:
        disassembleDir(instr->tokens[0], outputFile);
        break;
    case b:
        disassembleB(instr->instrname, instr->tokens[0], outputFile, PC);
        break;
    case ls:
        disassembleLS(instr->instrname, instr->tokens, numTokens, outputFile, PC);
        break;
    case nop:
        fwrite(&nopInstr, sizeof(int), 1, outputFile);
        break;
    case dp:
        disassembleDP(instr->instrname, instr->tokens, numTokens, outputFile);
        break;
    default:
        // Shouldn't reach here
        break;
    }
    // Update PC
    PC++;
}

void loadAssemblyFile(const char *filename, instruction *instr)
{
    // Check for proper assembly extension
    const char *extension = strrchr(filename, '.');
    if (!extension || strcmp(extension + 1, "s") != 0)
    {
        fprintf(stderr, "Not a valid assembly source file: %s\n.", filename);
        exit(EXIT_FAILURE);
    }

    FILE *file;
    file = fopen(filename, "r");

    if (!file)
    {
        perror("Could not open input file.\n");
        exit(EXIT_FAILURE);
    }

    // One Pass Approach
    PC = 0;
    while (fgets(instr->buff, BUFFER_LENGTH, file))
    {
        // Remove empty line
        instr->buff[strlen(instr->buff) - 1] = '\0';
        // Check that buff is not empty
        if (*instr->buff != '\0')
            disassembleRouter(instr);
    }

    fclose(file);
}

void openBinaryFile(const char *filename)
{
    outputFile = fopen(filename, "wb");
    if (outputFile == NULL)
    {
        perror("Failed to open output file.\n");
        exit(EXIT_FAILURE);
    }
    // Check for proper binary file extension
    const char *extension = strrchr(filename, '.');
    if (!extension || strcmp(extension, ".bin") != 0)
    {
        fprintf(stderr, "Not a valid binary file: %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    // Instructions are written to the output file once they are disasssembled
}

int main(int argc, char **argv)
{	
    char *inputFilename;
    char *outputFilename;
    if (argc >= 3)
    {
        inputFilename = argv[1];
        outputFilename = argv[2];
    }
    else
    {
        fprintf(stderr, "Provide both an input and an output file.\n");
        return EXIT_FAILURE;
    }
	// Initializing symtable, unDefLables
	symtable = initializeVector(MAX_INSTRS, sizeof(struct symbolTable));
	unDefLables = initializeVector(MAX_INSTRS, sizeof(struct labelMap));

    instruction *instr = initializeStruct();

    // Prepare output file for writing
    openBinaryFile(outputFilename);

    // Read from assembly file and disassemble each line
    loadAssemblyFile(inputFilename, instr);

    // One-pass: Compute previous undefined instructions
    handleUnDefLabels(outputFile);

    freeStruct(instr);
	
	freeVector(symtable);
   	freeVector(unDefLables);

	// Close output file
    fclose(outputFile);

    return EXIT_SUCCESS;
}
