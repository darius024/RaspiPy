#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdint.h>
#include "vector.h"

#define MAX_TOKEN_LENGTH 20
#define MAX_INSTRS 200
#define NUM_TOKENS 5

// #define SIZE_ALIASES (sizeof(aliases) / sizeof(char *))
// #define SIZE_WIDEMOVES (sizeof(wideMoves) / sizeof(char *))
// #define SIZE_ARITHMETICS (sizeof(arithmetics) / sizeof(char *))
// #define SIZE_LOGICAL (sizeof(logical) / sizeof(char *))
// #define SIZE_MULTIPLY (sizeof(multiply) / sizeof(char *))

// #define SIZE_DPI (sizeof(dpiOnes) / sizeof(int))
// #define SIZE_DPR (sizeof(dprOnes) / sizeof(int))

struct symbolTable
{
    char label[MAX_TOKEN_LENGTH];
    int address;
};


//enum type for possible undefined label cases
typedef enum {
    ll, //load literal 0
    bu, // branch unconditional 1 
    bc // branch conditional 2
 } unDefTypes;

//one pass structure
struct labelMap {
    char label[MAX_TOKEN_LENGTH];
    long offset;
    uint32_t instr;
    unDefTypes type;
};


//extern struct labelMap unDefLables[MAX_INSTRS];
extern vector *unDefLables;

//extern struct symbolTabl symtable[MAX_INSTRS]
extern vector *symtable;

#endif
