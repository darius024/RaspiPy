#ifndef TESTIR_H
#define TESTIR_H

#include <stdint.h>
#include "ast.h"
#include "ir.h"

// Function to convert TokenType to string
const char* token_type_to_string(TokenType type);

// Function to print a token
void print_token(Token *token);

// Function to convert IRType to string
const char* ir_type_to_string(IRType type);

// Function to print an IR instruction
void print_ir_instruction(IRInstruction *instr);

// Function to print an IR program
void print_ir_program(IRProgram *program);

#endif
