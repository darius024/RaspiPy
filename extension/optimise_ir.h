#ifndef OPTIMISE_IR_H
#define OPTIMISE_IR_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ast.h"
#include "state.h"

extern Expression *const_prop(Expression *expr, State *state); 
extern Expression *const_fold(Expression *expr, State *state);
extern void simplify_if_statement(IfStmt *if_stmt, State *state);
extern void simplify_statement(Statement *statement, State *state);
extern void simplify_statements(Statements *statements, State *state);
#endif
