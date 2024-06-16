#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"

IRProgram ast_to_ir(Program program)
{
    // TODO
    // We do here the (pre, mid, post) instruction handling
    // by executing the fields in each node of the AST successively

}

IRProgram stmts_to_ir(Stmts stmts)
{
    // TODO
    // Execute a block... construct a list which we insert in the main program
}

