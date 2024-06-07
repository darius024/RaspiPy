#include "onepass.h"

#define SIZE_DP (sizeof(dataProcessing) / sizeof(char *))
#define SIZE_LS (sizeof(loadAndStore) / sizeof(char *))
#define SIZE_B (sizeof(branching) / sizeof(char *))
#define SIZE_ALS (sizeof(aliases) / sizeof(char *))
#define SIZE_DIR (sizeof(directive) / sizeof(char *))
#define LABEL_ID ':'

#define SIZE_ARITHMETICS (sizeof(arithmetics) / sizeof(char *))
#define SIZE_LOGICAL (sizeof(logical) / sizeof(char *))
#define SIZE_WIDEMOVES (sizeof(wideMoves) / sizeof(char *))
#define SIZE_MULTIPLY (sizeof(multiply) / sizeof(char *))

#define SIZE_DPI1 (sizeof(dpiOnes) / sizeof(int))
#define SIZE_DPR1 (sizeof(dprOnes) / sizeof(int))
#define SIZE_LS1 (sizeof(lsOnes) / sizeof(int))
#define SIZE_SDT1 (sizeof(sdtOnes) / sizeof(int))
#define SIZE_SDTREG1 (sizeof(sdtRegOnes) / sizeof(int))
#define SIZE_B1 (sizeof(bOnes) / sizeof(int))

#define MAX_INSTR (2 * 1024 * 1024) // 2MB

// Store binary representation - more efficient than rewriting to IO
extern uint32_t binaryInstr[MAX_INSTR];
// Keep track of address of instruction executed
extern int PC;

// Mnemonic keywords
const char *dataProcessing[] = {
    "add", "adds", "sub", "subs",
    "and", "ands", "bic", "bics", "eor", "orr", "eon", "orn",
    "movk", "movn", "movz",
    "madd", "msub"};
const char *loadAndStore[] = {
    "str", "ldr"};
const char *branching[] = {
    "b", "br", "b.eq", "b.ne", "b.ge", "b.lt", "b.gt", "b.le", "b.al"};
const char *aliases[] = {
    "cmp", "cmn", "neg", "negs", "tst", "mvn", "mov", "mul", "mneg"};
const char *aliasesName[] = {
    "subs", "adds", "sub", "subs", "ands", "orn", "orr", "madd", "msub"};

const char *directive[] = {
    ".int"};
const char *shifts[] = {
    "lsl", "lsr", "asr", "ror"};

const char *arithmetics[] = {
    "add", "adds", "sub", "subs"};
const char *logical[] = {
    "and", "bic", "orr", "orn", "eor", "eon", "ands", "bics"};
const char *wideMoves[] = {
    "movn", "", "movz", "movk"};
const char *multiply[] = {
    "madd", "msub"};
const char *multiply[] = {
    "madd", "msub"};

// Specific 1 patterns in instructions
static const int dpiOnes[] = {28};
static const int dprOnes[] = {25, 27};
static const int lsOnes[] = {27, 28};
static const int bOnes[] = {26, 28};

static const int sdtOnes[] = {29};
static const int sdtRegOnes[] = {11, 13, 14};


// Type declarations
enum type {
    dp,  // data processing
    ls,  // load/store
    b,   // branch
    als,  // alias
    dir, // directive
    lb   // label
};

enum dpType {
    imm, // immediate
    reg, // register
};
// Only the third token of each DP instruction is needed to identify this

struct symbolTable {
    char label[MAX_TOKEN_LENGTH];
    int address;
};