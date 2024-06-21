#ifndef HOTSPOT_H
#define HOTSPOT_H

#include <stdlib.h>
#include <stdint.h>
#include "ir.h"


// Prototypes
uint32_t getHotSpot(IRInstruction *ir_instr);
uint32_t returnHotSpot(int line);
void addHotSpot(uint32_t instrBin, int line);

#endif
