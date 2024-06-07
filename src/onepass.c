#include "datatypes_as.h"

#include "datatypes_as.h"
#include "instructions.h"
#include "utils_as.h"

extern uint32_t binaryInstr;
extern size_t PC;

void handleUndefLabel(int i)
{
    struct undefTable *entry = (struct undefTable *)getFromVector(undeftable, i);
    uint32_t instruction = binaryInstr[entry->PC];

    int literal = getLiteral(entry->label);
    int offset = (literal - entry->PC) >> 2;

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

// Iterates through vector to handle all the undefined lables
void handleUndefTable()
{
    for (int i = 0; i < undeftable->currentSize; i++) {
        handleUndefLabel(i);
    }
}

void updateUndefTable(enum undefType type, char *labelName)
{
    struct undefTable *newEntry = (struct undefTable *)malloc(sizeof(struct undefTable));
    
    newEntry->PC = PC;
    newEntry->type = type;
    strcpy(newEntry->label, labelName);

    addToVector(undeftable, newEntry);
}