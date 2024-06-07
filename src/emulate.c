#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "constants.h"
#include "datatypes_em.h"
#include "decoders.h"
#include "execute.h"
#include "instructions.h"
#include "io.h"
#include "utils_em.h"

// Emulator State
extern struct EmulatorState state;

// Utility Functions
void updatePC(void)
{
    state.PC += INSTR_BYTES;
}

void initializeState(void)
{
    memset(&state, 0, sizeof(struct EmulatorState));
    state.pstate.Z = true;
}

//
// Pipeline Stages
//
uint32_t fetch(uint32_t addr)
{
    // Fetch instruction from memory
    uint32_t result = 0;
    for (int i = 0; i < INSTR_BYTES; i++) {
        result |= ((uint32_t)state.mem[addr + i]) << (BYTE_SIZE * i);
    }
    // The value is read from little endian memory
    return result;
}

int execute(Instruction instruction)
{
    switch (instruction.instructionType) {
        case isDPI:
            return executeDPI(instruction);
        case isDPR:
            return executeDPR(instruction);
        case isSDT:
            return executeSDT(instruction);
        case isB:
            return executeB(instruction);
        default:
            perror("Unsupported instruction type.\n");
            return EXIT_FAILURE;
    }
}

//
// IO Handling
//
void readToMemory(FILE *file)
{
    size_t numberOfBytes = fread(state.mem, 1, MEMORY_SIZE, file);
    if (numberOfBytes == 0) {
        perror("The file is empty.");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

void writeFinalState(FILE *file)
{
    fprintf(file, "Registers:\n");
    for (int i = 0; i < NUM_OF_REGISTERS; i++) {
        fprintf(file, "X%d%d    = %016lx\n", i / 10, i % 10, state.R[i]);
    }
    fprintf(file, "PC     = %016lx\n", state.PC);
    fprintf(file, "PSTATE : %c%c%c%c",
            state.pstate.N ? 'N' : '-',
            state.pstate.Z ? 'Z' : '-',
            state.pstate.C ? 'C' : '-',
            state.pstate.V ? 'V' : '-');
    fprintf(file, "\nNon-Zero Memory:\n");
    for (int addr = 0; addr < MEMORY_SIZE; addr += INSTR_BYTES) {
        uint32_t binInstr = fetch(addr);
        if (binInstr != 0) {
            fprintf(file, "0x%08x : %08x\n", addr, binInstr);
        }
    }
}

//
// Main Program
//
int main(int argc, char **argv)
{
    char *inputFile;
    char *outputFile;
    if (argc >= 2) {
        inputFile = argv[1];
        outputFile = (argc > 2) ? argv[2] : SDTOUT;
    } else {
        perror("Provide at least an input file.\n");
        exit(EXIT_FAILURE);
    }

    // Set up initial state
    initializeState();

    // Initializing data types
    uint32_t instr;
    Instruction *instruction = initializeInstruction();

    // Store instructions into memory
    FILE *input = loadInputFile(inputFile, "bin", "rb");
    readToMemory(input);

    while ((instr = fetch(state.PC)) != HALT_INSTR) {
        int decodeError = decode(&instr, instruction, getBits);
        checkError(decodeError);
        int executeError = execute(*instruction);
        checkError(executeError);
    }

    // Free data types
    freeInstruction(instruction);

    // Write the final state after executing all instructions
    FILE *output = openOutputFile(outputFile, "out", "w");
    writeFinalState(output);

    // Close files
    closeFiles(input, output);

    return EXIT_SUCCESS;
}
