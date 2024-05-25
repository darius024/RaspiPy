#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MEMORY_SIZE (2 * 1024 * 1024)

// Memory
unsigned char mem[MEMORY_SIZE];

// Emulator State
struct estate
{
    long R[31];
    long ZR;
    long PC;
    long SP;
    struct PSTATE
    {
        bool N;
        bool Z;
        bool C;
        bool V;
        // int mode;
    } pstate;
};
struct estate state = {0};

// Categories of instructions to support
typedef enum
{
    DPImm,
    DPReg,
    LS,
    B,
    Err
} category;

//
// Utility Functions
//

void initializeState(void)
{
    state.pstate.Z = true;
}

void charToBinary(char c, char *memData)
{
    for (int i = 7; i >= 0; i--)
    {
        memData[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
    memData[8] = '\0';
}

int instructionToInt(long addr)
{
    int result = 0;
    result |= mem[addr];
    result |= (mem[addr + 1] << 8);
    result |= (mem[addr + 2] << 16);
    result |= (mem[addr + 3] << 24);
    // This is also transofrmed from little endian
    return result;
}

void loadBinaryFile(const char *filename)
{
    if (strcmp(strrchr(filename, '.') + 1, "bin") != 0)
    {
        perror("This is not a valid binary file.");
        exit(EXIT_FAILURE);
    }
    FILE *file;
    file = fopen(filename, "rb");
    if (!file)
    {
        perror("File not found.");
        exit(EXIT_FAILURE);
    }

    size_t numberOfBytes = fread(mem, 1, MEMORY_SIZE, file);

    if (numberOfBytes == 0)
    {
        perror("The file is empty.");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void writeOutput(const char *filename)
{
    FILE *file;
    file = (!strcmp(filename, "stdout")) ? stdout : fopen(filename, "a");
    if (!file)
    {
        perror("File not found.");
        exit(EXIT_FAILURE);
    }
    // if (filename != stdout && strcmp(strrchr(filename, '.') + 1, "out") != 0)
    // {
    //     perror("A .out file should be provided instead.");
    //     exit(EXIT_FAILURE);
    // }

    fprintf(file, "Registers:\n");
    for (int i = 0; i < 31; i++)
    {
        fprintf(file, "X%d%d    = %016lx\n", i > 9 ? i / 10 : 0, i % 10, state.R[i]);
    }
    fprintf(file, "PC     = %016lx\n", state.PC);
    fprintf(file, "PSTATE : %c%c%c%c",
            state.pstate.N ? 'N' : '-',
            state.pstate.Z ? 'Z' : '-',
            state.pstate.C ? 'C' : '-',
            state.pstate.V ? 'V' : '-');
    fprintf(file, "\nNon-zero memory:\n");
    for (int addr = 0; addr < MEMORY_SIZE; addr += 4)
    {
        int binInstr = instructionToInt(addr);
        if (binInstr != 0)
        {
            fprintf(file, "0x%08x: %08x\n", addr, binInstr);
        }
    }
    fclose(file);
}

//
// Binary Manipulation Helpers
//
int checkOp0(long instr)
{
    return instr >> 25 & 0x15;
}

void getBits(uint32_t x, int nbits, char *buf)
{
    uint32_t mask = 1 << (nbits - 1);
    for (int i = 0; i < nbits; i++)
    {
        int bit = (x & mask) != 0;
        buf[nbits - i - 1] = '0' + bit;
        mask >>= 1;
    }
}

//
// Pipeline Stages
//
long fetch(void)
{
    // Fetch instruction from memory
    long instruction = instructionToInt(state.PC);

    // Compute the address of the next instruction
    state.PC = state.PC + 4;
    // We update PC for branches after execution

    return instruction;
}

category decode(long instr)
{
    char op0[4];
    getBits(checkOp0(instr), 4, op0);
    if (op0[1] == 0)
    {
        if (op0[0] == 1)
        {
            return (op0[2] == 0) ? DPImm : B;
        }
    }
    else
    {
        if (op0[3] == 1)
        {
            if (op0[2] == 0)
                return DPReg;
        }
        else
            return LS;
    }
    return Err;
}

//
// Main Program
//

int main(int argc, char **argv)
{
    char *inputFile = argv[1];
    char *outputFile = (argc > 2) ? argv[2] : "stdout";

    // Set up initial state
    initializeState();
    // Read from binary file
    // Store instructions into memory
    loadBinaryFile(inputFile);

    // 0x8a000000 in hex = 2315255808 in dec
    while (state.PC != 2315255808)
    {
        long instr = fetch();
        category cat = decode(instr);

        switch (cat)
        {
        case DPImm:
            // Code
            break;
        case DPReg:
            // Code
            break;
        case LS:
            // Code
            break;
        case B:
            // Code
            break;
        default:
            // Incorrect opcode
            perror("Incorrect opcode0, instruction not supported");
            exit(EXIT_FAILURE);
        }
    }

    // Write the final state after executing all instructions
    writeOutput(outputFile);
    return EXIT_SUCCESS;
}
