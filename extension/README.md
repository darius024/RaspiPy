# Extension - C Group Project

# AOT Optimized Compiler for a Python-like language, called RaspiPy
# RaspiPy enables most of the blocks Python has, but provides additional
# support for working with Raspberry Pis as well, especially easy GPIO access

# Implementation
1. Lexer - tokens of the language
2. Parser - grammar / rules that the language supports
3. Abstract Syntax Tree (AST):
    - build while parsing the input
    - constructed from multiple data structures with well-defined children
    - expressions / statements are represented as tagged unions
    - can be traversed starting from a program
4. Intermediary Representation (IR):
    - an IR instruction an assembly line: mnemonic(type) + tokens
    - a program is represeted as a linked-list of instructions
    - the instruction holds profiling information as well
    - state: map of variables, functions and labels symbol table, directives, stack
5. Assembly code:
    - easily generated from an IR instruction

# Registers / Stack Discipline
- complies with the ARM calling conventions
- X0-X7 are reserved for passing parameters
- X0 is reserved for passsing the return address
- X8-X30 are saved registes
- X30 is reserved for the return address
- registers are pushed to the stack and popped in reverse order

# Optimizations
1. Constant folding
2. Constant propagation
3. Registers are used x- or w- accordingly
4. Function inline
