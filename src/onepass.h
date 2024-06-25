// Data types and functions to handle one-pass for the assembler

#ifndef ONEPASS_H
#define ONEPASS_H

#include <stdint.h>
#include "constants.h"
#include "vector.h"


// Types of possible undefined label cases
enum undefType {
    ll, // load literal
    bu, // branch unconditional
    bc  // branch conditional
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
extern void handleUndefTable(void);
extern void updateUndefTable(enum undefType type, char *labelName);

#endif
