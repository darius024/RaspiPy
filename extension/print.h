#ifndef PRINT_H
#define PRINT_H

#include <stdbool.h>
#include <stdint.h>
#include "ast.h"
#include "ir.h"

void print_token(State *state, Token *token, FILE *output);
void print_ir_instruction(IRInstruction *instr, State *state, FILE *output);
void print_ir_program(IRProgram *program, State *state, FILE *output);
void print_ast(Statements *statements, int indent);
void print_program(Program *program);

#endif
