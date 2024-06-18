#ifndef OPTIMISE_IR_H
#define OPTIMISE_IR_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"
#include "utlis_ir.c"
#include "eval.c"

//returns pointer to the expression
Expression *const_prop(Expression *expr, state *State); 
Expression *const_fold(Expression *expr, state *State);
#endif
