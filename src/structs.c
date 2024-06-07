#include <stdlib.h>

#include "structs.h"

Instruction *initializeInstruction() 
{
    Instruction *instruction = (Instruction *)malloc(sizeof(Instruction));

    if (instruction == NULL) {
        perror("Failed to allocate space for Instruction struct.\n");
        exit(EXIT_FAILURE);
    }
    return instruction;
}

void freeInstruction(Instruction *instruction) 
{
    free(instruction);
}

InstructionParse *initializeInstructionParse()
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

void freeInstructionParse(InstructionParse *instructionParse)
{
    for (int i = 0; i < NUM_TOKENS; i++) {
        free(instructionParse->tokens[i]);
    }
    free(instructionParse);
}