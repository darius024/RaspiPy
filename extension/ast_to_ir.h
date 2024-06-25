#ifndef AST_TO_IR_H
#define AST_TO_IR_H

#include "ast.h"
#include "ir.h"

IRProgram *ast_to_ir(Program *prog, State *state);

#endif
