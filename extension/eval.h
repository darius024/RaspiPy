#ifndef EVAL_H
#define EVAL_H

#include <stdint.h>
#include "ast.h"
#include "ir.h"

// Prototypes
uint8_t eval_expression(IRProgram *program, Expression *expression, State *state, int *line, int count_update);

#endif 
