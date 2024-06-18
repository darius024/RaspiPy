#ifndef OPTIMISE_IR_H
#define OPTIMISE_IR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ast.h"
#include "ir.h"

Expression *const_prop(Expression *expr, State *state); 
Expression *const_fold(Expression *expr, State *state);

#endif
