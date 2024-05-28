
#include "../assemble.h"

// Function Prototypes
int getMode(char *rd);

int getInt(char *val);

int getImmediate(char *imm);

int getRegister(char *rd);

int getOperand(char *op, bool *rOrImm);

int getLiteral(char *literal);

int getShift(char *shift);

int maskBetweenBits(int upp, int low);