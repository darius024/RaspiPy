#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <stdint.h>
#include "ir.h"

#define MAX_HOTSPOTS 128
#define MAX_ASSEMBLY_LINE 128
#define THRESHOLD 16

typedef struct {
    int line;
    uint32_t instruction;
} HotMap;


// Prototypes
uint32_t getHotSpot(IRInstruction *ir_instr);
void addHotSpot(uint32_t instrBin, int line);

#endif
