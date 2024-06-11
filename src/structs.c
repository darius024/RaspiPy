#include <assert.h>
#include <stdlib.h>
#include "structs.h"


// Malloc / Free Struct
Instruction *initializeInstruction(void) 
{
    Instruction *instruction = malloc(sizeof(Instruction));
    assert(instruction != NULL);
    return instruction;
}

void freeInstruction(Instruction *instruction) 
{
    free(instruction);
}