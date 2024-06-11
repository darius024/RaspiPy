// Assembler data types and structures

#ifndef DATATYPES_AS_H
#define DATATYPES_AS_H

#include <stdint.h>

#define DATA_PROCESSING_SIZE 17
#define LOAD_AND_STORE_SIZE 2
#define BRANCHING_SIZE 9
#define ALIASES_SIZE 9
#define ALIASES_NAME_SIZE 9
#define DIRECTIVE_SIZE 1
#define SHIFTS_SIZE 4
#define ARITHMETICS_SIZE 4
#define LOGICAL_SIZE 8
#define WIDE_MOVES_SIZE 4
#define MULTIPLY_SIZE 2

#define BUFFER_LENGTH 256
#define NUM_INSTRS 256
#define NUM_TOKENS 5
#define MAX_TOKEN_LENGTH 20

#define SPACE " "
#define SPACECOMMA ", "
#define LABEL_ID ':'
#define END_OF_FILE 0
#define IN_FILE 1


extern uint32_t binaryInstr[NUM_INSTRS];
extern int PC;

// Mnemonic keywords
extern const char *dataProcessing[];
extern const char *loadAndStore[LOAD_AND_STORE_SIZE];
extern const char *branching[BRANCHING_SIZE];
extern const char *aliases[ALIASES_SIZE];
extern const char *aliasesName[ALIASES_NAME_SIZE];
extern const char *directive[DIRECTIVE_SIZE];
extern const char *shifts[SHIFTS_SIZE];
extern const char *arithmetics[ARITHMETICS_SIZE];
extern const char *logical[LOGICAL_SIZE];
extern const char *wideMoves[WIDE_MOVES_SIZE];
extern const char *multiply[MULTIPLY_SIZE];

enum type
{
    dp,  // data processing
    ls,  // load/store
    b,   // branch
    als, // alias
    dir, // directive
    lb   // label
};

enum dpType
{
    imm, // immediate
    reg  // register
};

struct symbolTable
{
    char label[MAX_TOKEN_LENGTH];
    int address;
};

typedef struct
{
    enum type type;
    int numTokens;
    char *tokens[NUM_TOKENS];
    char instrname[MAX_TOKEN_LENGTH];
    char buff[BUFFER_LENGTH];
} InstructionParse;

#endif