#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

int instructionToInt(unsigned char c0, unsigned char c1, unsigned char c2, unsigned char c3) {
    int result = 0;
    result |= c0;
    result |= (c1 << 8);
    result |= (c2 << 16);
    result |= (c3 << 24);
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
        int binInstr = instructionToInt(mem[addr], mem[addr + 1], mem[addr + 2], mem[addr + 3]);
        if (binInstr != 0)
        {
            fprintf(file, "0x%08x: %08x\n", addr, binInstr);
        }
    }
    fclose(file);
}

int main(int argc, char **argv)
{
    char *inputFile = argv[1];
    char *outputFile = (argc > 2) ? argv[2] : "stdout";

    initializeState();
    loadBinaryFile(inputFile);
    writeOutput(outputFile);

    return EXIT_SUCCESS;
}
