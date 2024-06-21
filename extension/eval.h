#ifndef EVAL_H
#define EVAL_H

#include <stdint.h>
#include "ast.h"
#include "ir.h"

// Prototypes
uint8_t evalExpression(IRProgram *program, Expression *expression, State *state, int *line, int count_update);

#endif 
