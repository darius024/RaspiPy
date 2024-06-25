#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "datatypes_as.h"
#include "instructions.h"
#include "onepass.h"
#include "utils_as.h"


extern uint32_t binaryInstr[NUM_INSTRS];
extern int PC;

// Complete the instruction with the field necessary
static void handleUndefLabel(int i)
{
    struct undefTable *entry = getFromVector(undeftable, i);
    uint32_t instruction = binaryInstr[entry->PC];

    int literal = getLiteral(entry->label, symtable);
    int offset = (literal - entry->PC * INSTR_BYTES) / INSTR_BYTES;

    switch (entry->type) {
        case ll: // Load Literal
            putBits(&instruction, &offset, SDT_SIMM19_OFFSET, SDT_SIMM19_LEN);
            break;
        case bc: // Branch Conditional
            putBits(&instruction, &offset, B_SIMM19_OFFSET, B_SIMM19_LEN);
            break;
        case bu: // Branch Unconditional
            putBits(&instruction, &offset, B_SIMM26_OFFSET, B_SIMM26_LEN);
            break;
    }
    binaryInstr[entry->PC] = instruction;
}

// Iterates through all the undefined lables and completes the instructions
void handleUndefTable(void)
{
    for (int i = 0; i < undeftable->currentSize; i++) {
        handleUndefLabel(i);
    }
}

// Update information about the instructions that need to be revised
void updateUndefTable(enum undefType type, char *labelName)
{
    struct undefTable *newEntry = malloc(sizeof(struct undefTable));
    assert(newEntry != NULL);
    newEntry->PC = PC;
    newEntry->type = type;
    strcpy(newEntry->label, labelName);
    addToVector(undeftable, newEntry);
}
