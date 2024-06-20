#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "datatypes_as.h"
#include "decoders.h"
#include "disassembler.h"
#include "io.h"
#include "onepass.h"
#include "structs.h"
#include "utils_as.h"
#include "vector.h"


// Record binary representation pf instructions
uint32_t binaryInstr[NUM_INSTRS];
int PC = 0;

// symtable stores symbolTables as elements
vector *symtable;
// undefLables stores labelMaps as elements
vector *undeftable;

//
// Pipeline Stages
//
// Decompose an instruction into its correspoding type and tokens
int decompose(InstructionParse *instr, FILE *input)
{                     
    if (!fgets(instr->buff, BUFFER_LENGTH, input)) {
        return END_OF_FILE;
    }
    instr->buff[strlen(instr->buff) - 1] = '\0';
    if (isspace(*(instr->buff))) {
        return IN_FILE;
    }

    // Save pointer for strtok_r
    char *instrSavePntr = NULL;

    // Set the mnemonic and type of the instruction
    char *token = strtok_r(instr->buff, SPACE, &instrSavePntr);
    strcpy(instr->instrname, token);
    instr->type = identifyType(instr->instrname);

    // Take the tokens one by one
    instr->numTokens = 0;
    token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    while (token != NULL) {
        strcpy(instr->tokens[instr->numTokens++], token);
        token = strtok_r(NULL, SPACECOMMA, &instrSavePntr);
    }
    return IN_FILE;
}

//
// IO Handling
//
static void writeBinaryInstr(FILE *file)
{
    fwrite(binaryInstr, sizeof(uint32_t), PC, file);
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
        EXIT_PROGRAM("Provide at least an input file.");
    }

	// Initializing data types
    Instruction *instruction = initializeInstruction();
    InstructionParse *instructionParse = initializeInstructionParse();
	symtable = initializeVector(NUM_INSTRS, sizeof(struct symbolTable));
	undeftable = initializeVector(NUM_INSTRS, sizeof(struct undefTable));

    // Read from assembly file and disassemble each line
    FILE *input = loadInputFile(inputFile, NULL, "r");

    while(decompose(instructionParse, input) != END_OF_FILE) {
        if (*(instructionParse->buff) == '\0') continue; // empty line

        int disassembleError = disassemble(instructionParse, instruction);
        checkError(disassembleError);
        
        if (instructionParse->type == lb || instructionParse->type == dir) continue; // no decode
        uint32_t instr = 0;
        int decodeError = decode(&instr, instruction, putBits);
        checkError(decodeError);

        addBinaryInstr(instr);
    }

    // One-pass: Compute previous undefined instructions
    handleUndefTable();

    // Free data types
    freeInstructionParse(instructionParse);
    freeInstruction(instruction);
	freeVector(symtable);
   	freeVector(undeftable);

    // Write the binary instructions
    FILE *output = openOutputFile(outputFile, NULL, "wb");
    writeBinaryInstr(output);

    // Close files
    closeFiles(input, output);

    return EXIT_SUCCESS;
}
