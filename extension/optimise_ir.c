#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ir.h"
#include "ast.h"
#include "optimise_ir.h"
#include "utils_ir.h"

//returns pointer to the expression
Expression *const_prop(Expression *expr, State *state) {
    switch (expr->tag)
    {
    case (EXPR_NAME):
        Int *val = malloc(sizeof(Int));
        int64_t *var = search_vars(expr->name->name, state);
        if (var == NULL){
            free_int(val);
            perror("couldnt evaluate the binary op");
            exit(EXIT_FAILURE);
        }
        free_name(expr->name);
        expr->tag = EXPR_INT;
        expr->int_value = val;
        expr->int_value->value = *var;
        return expr;    
    case (EXPR_INT):
        return expr;
    case (EXPR_BINARY_OP):
        Expression l = *const_prop(expr->binary_op->left, state);
        Expression r = *const_prop(expr->binary_op->right, state);

        *expr->binary_op->left = l;
        *expr->binary_op->right = r;

        return expr;
    case (EXPR_UNARY_OP):
        Expression *e = const_prop(expr->unary_op->expression, state);

        expr->unary_op->expression = e;
        return expr; 
    case (EXPR_FUNCTION_CALL):
        Arguments* start = expr->function_call->args;
        while (start != NULL) {
            const_prop(start->arg, state);
            start = start->next;
        }
        return expr;
    default: //error
        return NULL;
    }
    
}

Expression *const_fold(Expression *expr, State *state) {
    switch (expr->tag)
    {
    case (EXPR_NAME):
        return const_prop(expr, state);
    case (EXPR_INT):
        return expr;
    case (EXPR_BINARY_OP):
        Expression l = *const_fold(expr->binary_op->left, state);
        Expression r = *const_fold(expr->binary_op->right, state);
        
        if (l.tag == EXPR_INT && r.tag == EXPR_INT) {
            Int *val = malloc(sizeof(Int));
            int64_t *var = evaluate_int_binary_op(expr->binary_op->op, l.int_value->value, r.int_value->value);
            if (var == NULL) {
                free_int(val);
                perror("couldnt evaluate the binary op");
                exit(EXIT_FAILURE);
            } 
            free_binary_op(expr->binary_op);
            expr->tag = EXPR_INT;
            expr->int_value = val;
            expr->int_value->value = *var;  
            return expr;          
        }
        *expr->binary_op->left = l;
        *expr->binary_op->right = r;
        return expr;
    case (EXPR_UNARY_OP):
        Expression u = *const_fold(expr->unary_op->expression, state);

        if (expr->tag == EXPR_INT) {
            Int *val = malloc(sizeof(Int));
            int64_t *var = evaluate_int_unary_op(expr->unary_op->op, u.int_value->value);
            if (var == NULL) {
                free_int(val);
                perror("couldnt evaluate the binary op");
                exit(EXIT_FAILURE);
            } 
            free_unary_op(expr->unary_op);
            expr->tag = EXPR_INT;
            expr->int_value = val;
            expr->int_value->value = *var;  
            return expr;          
        }

        *expr->unary_op->expression = u;
        return expr;
    case (EXPR_FUNCTION_CALL):
        Arguments* start = expr->function_call->args;
        while (start != NULL) {
            const_fold(start->arg, state);
            start = start->next;
        }
        return expr;            
    default: // Function call
        perror("invalid tag"); 
        exit(EXIT_FAILURE);
        break;
    }
    
}
