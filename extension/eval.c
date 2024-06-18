#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"
#include "eval.h"
#include "utlis_ir.c"


static int64_t *evaluate_int_binary_op(const char *op, int64_t left, int64_t right) {
    int64_t *v = malloc(sizeof(int64_t));
 
    if (strcmp(op, "+") == 0) {*v = left + right;}
    else if (strcmp(op, "-") == 0) {*v = left - right;}
    else if (strcmp(op, "*") == 0) {*v = left * right;}
    else if(strcmp(op,"|") == 0)   {*v = left | right;}
    else if(strcmp(op,"^") == 0)   {*v = left ^ right;} 
    else if(strcmp(op,"&") == 0)   {*v = left & right;} 
    else if(strcmp(op,"<<") == 0)  {*v = left << right;} 
    else if(strcmp(op,">>") == 0)  {*v = left >> right;}
    else if (strcmp(op, "/") == 0 && right != 0) {*v = left / right;} // Avoid division by zero
    else if ('%' == *op && right != 0) {*v =  left % right;} // Avoid modulo by zero
    else if(strcmp(op,"<") == 0)  { *v =  (left < right)? 1 : 0;} 
    else if(strcmp(op,">") == 0)  { *v =  (left > right)? 1 : 0;}
    else if(strcmp(op,"==") == 0) { *v =  (left == right)? 1 : 0;} 
    else if(strcmp(op,"!=") == 0) { *v =  (left != right)? 1 : 0;}
    else if(strcmp(op,"<=") == 0) { *v =  (left <= right)? 1 : 0;} 
    else if(strcmp(op,">=") == 0) { *v =  (left >= right)? 1 : 0;}
    else if(strcmp(op,"and") == 0) { *v =  (left && right)? 1 : 0;} 
    else if(strcmp(op,"or") == 0) {*v =  (left || right)? 1 : 0;}
    else {v = NULL; free(v);}
    
    return v;
}

static int64_t *evaluate_int_unary_op(const char *op, int64_t operand) {
    int64_t *v = malloc(sizeof(int64_t));

    if (strcmp(op, "-") == 0) {*v = -operand;}
    else if (strcmp(op, "+") == 0) {*v = +operand;}
    else if (strcmp(op, "~") == 0) {*v = ~operand;}
    else {v = NULL; free(v);}
    
    return v; 
}

int64_t EvalExpression(Expression *expression, State *state)
{
    //TODO - Sajeshes code
    switch (expression->tag)
    {
    case (EXPR_NAME):
        int64_t* n = search_vars(expression->name->name, state);
        if (n != NULL) {
            return *n;
        }
        break;  
    case (EXPR_INT):
        int64_t* i  = expression->int_value->value;
        if (i != NULL) {
            return *i;
        }
        break;  
    case (EXPR_BINARY_OP):
        int64_t *l_val;
        int64_t *r_val;

        l_val = EvalExpression(expression->binary_op->left, state);
        r_val = EvalExpression(expression->binary_op->right, state);

        if (l_val != NULL && r_val !=NULL) {
            return evaluate_int_binary_op(expression->unary_op->op, *l_val, *r_val);
        } 
        break;
    case (EXPR_UNARY_OP):
        int64_t *u_val;
        u_val = EvalExpression(expression->unary_op->expression, state);
        if (u_val != NULL) {
            return evaluate_int_unary_op(expression->unary_op->op, *u_val);
        } 
        break;
    case (EXPR_FUNCTION_CALL):
        break;            
    default:
        break;
    }

    return 0;

}
