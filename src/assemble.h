#include <stdint.h>
#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#define MAX_TOKEN_LENGTH 20
#define MAX_INSTRS 200

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


extern struct labelMap unDefLables[MAX_INSTRS];
extern int labelIdx;

extern struct symbolTable symtable[MAX_INSTRS];
extern int numLabel;

#endif