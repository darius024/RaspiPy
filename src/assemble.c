#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assemble.h"
#include "disassembler/disassembler.h"
#include "utils/assembler_utils.h"

#define NUM_TOKENS 5
#define SPACE " "
#define SPACECOMMA ", "
#define OPCODE 0
#define BUFFER_LENGTH 128
#define MAX_TOKEN_LENGTH 20
#define CPOINTER_SIZE sizeof(char *)
#define MAX_INSTRS 200

#define DP_SIZE sizeof(dataProcessing) / CPOINTER_SIZE
#define SINGLE_OPERAND_SIZE sizeof(singleOperand) / CPOINTER_SIZE
#define TWO_OPERAND_NO_DEST_SIZE sizeof(twoOperandNoDest) / CPOINTER_SIZE
#define B_SIZE sizeof(branching) / CPOINTER_SIZE
#define LS_SIZE sizeof(loadAndStore) / CPOINTER_SIZE
#define DIR_SIZE sizeof(dir) / CPOINTER_SIZE

// int instr[MAX_INSTRS];
int PC;
// Declare them global as they are needed during the entire execution of the program
char *tokens[NUM_TOKENS];
char instrname[MAX_TOKEN_LENGTH];
char buff[BUFFER_LENGTH];

FILE *outputFile;

struct symbolTable
{
    char label[MAX_TOKEN_LENGTH];
    int address;
    int *p;
} symtable[MAX_INSTRS];
int numLabel = 0;

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
const char *dataProcessing[] = {
    "add", "adds", "sub", "subs",
    "cmp", "cmn",
    "neg", "negs",
    "and", "ands", "bic", "bics", "eor", "orr", "eon", "orn",
    "tst",
    "movk", "movn", "movz", "mov", "mvn",
    "madd", "msub",
    "mul", "mneg"};

const char *branching[] = {
    "b", "br", "beq", "bne", "bge", "blt", "bgt", "ble", "bal"};

const char *loadAndStore[] = {
    "str", "ldr"};

const char *directive[] = {
    ".int"};

const char *nopOp = "nop";
const int NOP_INSTRUCTION = 0xd503201f;

// Use strtok_r instead of strtok
void decompose(char *instruction, int *numTokens)
{   // splits an instruction into its tokens
    char *token = strtok(instruction, SPACE); // ignores any indent at start
    // take the mnemonic of the instruction
    strcpy(instrname, token);
    token = strtok(NULL, SPACE);

    int count = 0;

    while (token != NULL)
    {
        strcpy(tokens[count], token);
        count++;
        token = strtok(NULL, SPACECOMMA);
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

void disassembleRouter(char *instruction)
{   
    // Sends instruction to corresponding disassembler
    int numTokens;
    // Construct the instrname and tokens array
    decompose(instruction, &numTokens);
    enum type instrType = identifyType(instrname);

    switch (instrType)
    {
    case lb:
        return; // Nothing to parse, No update of PC
    case dir:
        disassembleDir(tokens[0], outputFile);
        break;
    case b:
        disassembleB(instrname, tokens[0], outputFile, PC);
        break;
    case ls:
        disassembleLS(instrname, tokens, numTokens, outputFile, PC);
        break;
    case nop:
        fwrite(&NOP_INSTRUCTION, sizeof(int), 1, outputFile);
        break;
    case dp:
        disassembleDP(instrname, tokens, numTokens, outputFile);
        break;
    default:
        // Shouldn't reach here
        break;
    }
    // Update PC
    PC++;
}

void updateSymbolTable(char *buff)
{
    char *p = strchr(buff, ':');
    if (p != NULL)
    {
        // Label Case - Update the symbol table, no update of PC
        *p = '\0';
        strcpy(symtable[numLabel].label, buff);
        symtable[numLabel++].address = PC * 4;
    }
    else
        // Update PC to track the addresses of labels
        PC++;
}

void loadAssemblyFile(const char *filename)
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

    // Allocate memory for tokens
    // This way we only allocate memory once and use it for every instruction
    for (int i = 0; i < NUM_TOKENS; i++)
    {
        tokens[i] = (char *)malloc((MAX_TOKEN_LENGTH + 1) * sizeof(char));
        if (tokens[i] == NULL)
        {
            perror("Failed to allocate memory for tokens.\n");
            for (int j = 0; j < i; j++)
                free(tokens[j]);
            exit(EXIT_FAILURE);
        }
    }

    // Two Pass Approach

    // First Pass - Build symbol table
    PC = 0;
    while (fgets(buff, BUFFER_LENGTH, file))
    {
        // Remove empty line
        buff[strlen(buff) - 1] = '\0';
        // Check that buff is not empty
        if (*buff != '\0')
            updateSymbolTable(buff);
    }

    // Reset pointer to beginning for reread
    rewind(file);

    // Second Pass - Disassemble instructions
    PC = 0;
    while (fgets(buff, BUFFER_LENGTH, file))
    {
        // Remove empty line
        buff[strlen(buff) - 1] = '\0';
        // Check that buff is not empty
        if (*buff != '\0')
            disassembleRouter(buff);
    }

    for (int i = 0; i < NUM_TOKENS; i++)
        free(token[i]);

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

    // Prepare output file for writing
    openBinaryFile(outputFilename);

    // Read from assembly file and disassemble each line
    loadAssemblyFile(inputFilename);

    // Close output file
    fclose(outputFile);

    return EXIT_SUCCESS;
}
