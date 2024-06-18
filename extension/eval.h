#ifndef EVAL_H
#define EVAL_H

#include <stdint.h>
#include "AST.h"
#include "AST.h"
#include "utlis_ir.c"

// Function declarations
int64_t *evaluate_int_binary_op(const char *op, int64_t left, int64_t right);
int64_t *evaluate_int_unary_op(const char *op, int64_t operand);
int64_t *EvalExpression(Expression *expression, State *state);

#endif 