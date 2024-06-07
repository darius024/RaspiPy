#ifndef ONEPASS_H
#define ONEPASS_H

#include <stdint.h>

#include "vector.h"

#define BUFFER_LENGTH 200
#define MAX_INSTRS 200
#define NUM_TOKENS 5
#define MAX_TOKEN_LENGTH 20

// Enum type for possible undefined label cases
enum undefType {
    ll, // load literal 0
    bu, // branch unconditional 1 
    bc  // branch conditional 2
 };

// One Pass structure
struct undefTable {
    char label[MAX_TOKEN_LENGTH];
    int PC;
    enum undefType type;
};


// Prototypes
extern vector *undeftable;
extern vector *symtable;
extern void updateUndefTable(enum undefType type, char *labelName);

#endif