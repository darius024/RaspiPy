#include "datatypes_as.h"

// #define NELEMENTS(array) (sizeof(array) / sizeof(array[0]))

const char *dataProcessing[] = {
    "add", "adds", "sub", "subs",
    "and", "ands", "bic", "bics", "eor", "orr", "eon", "orn",
    "movk", "movn", "movz",
    "madd", "msub"
};

const char *loadAndStore[] = {
    "str", "ldr"
};

const char *branching[] = {
    "b", "br", "b.eq", "b.ne", "b.ge", "b.lt", "b.gt", "b.le", "b.al"
};

const char *aliases[] = {
    "cmp", "cmn", "neg", "negs", "tst", "mvn", "mov", "mul", "mneg"
};

const char *aliasesName[] = {
    "subs", "adds", "sub", "subs", "ands", "orn", "orr", "madd", "msub"
};

const char *directive[] = {
    ".int"
};

const char *shifts[] = {
    "lsl", "lsr", "asr", "ror"
};

const char *arithmetics[] = {
    "add", "adds", "sub", "subs"
};

const char *logical[] = {
    "and", "bic", "orr", "orn", "eor", "eon", "ands", "bics"
};

const char *wideMoves[] = {
    "movn", "", "movz", "movk"
};

const char *multiply[] = {
    "madd", "msub"
};
