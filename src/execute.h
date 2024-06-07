#include <stdint.h>
#include <stdbool.h>

// Execute Functions

extern struct EmulatorState state;

extern int executeDPI(Instruction instruction);

extern int executeDPR(Instruction instruction);

extern int executeSDT(Instruction instruction);

extern int executeB(Instruction instruction);