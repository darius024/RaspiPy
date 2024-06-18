#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ast.h"
#include "eval.h"
#include "ir.h"
#include "optimise_ir.h"
#include "utils_ir.h"

static int64_t evaluate_int_binary_op(const char *op, int64_t left, int64_t right)
{
    int64_t v;
 
    if (strcmp(op, "+") == 0) { v = left + right; }
    else if (strcmp(op, "-") == 0) { v = left - right; }
    else if (strcmp(op, "*") == 0) { v = left * right; }
    else if(strcmp(op, "|") == 0)   { v = left | right; }
    else if(strcmp(op, "^") == 0)   { v = left ^ right; } 
    else if(strcmp(op, "&") == 0)   { v = left & right; } 
    else if(strcmp(op, "<<") == 0)  { v = left << right; } 
    else if(strcmp(op, ">>") == 0)  { v = left >> right; }
    else if (strcmp(op,  "/") == 0 && right != 0) { v = left / right; } // Avoid division by zero
    else if (strcmp(op, "%") == 0 && right != 0) { v =  left % right; } // Avoid modulo by zero
    else if(strcmp(op, "<") == 0)  { v = (left < right) ? 1 : 0; } 
    else if(strcmp(op, ">") == 0)  { v = (left > right) ? 1 : 0; }
    else if(strcmp(op, "==") == 0) { v = (left == right) ? 1 : 0; } 
    else if(strcmp(op, "!=") == 0) { v = (left != right) ? 1 : 0; }
    else if(strcmp(op, "<=") == 0) { v = (left <= right) ? 1 : 0; } 
    else if(strcmp(op, ">=") == 0) { v = (left >= right) ? 1 : 0; }
    else if(strcmp(op, "and") == 0) { v = (left && right) ? 1 : 0; } 
    else if(strcmp(op, "or") == 0) { v = (left || right) ? 1 : 0; }
    else { exit(EXIT_FAILURE); }
    
    return v;
}

int64_t evaluate_int_unary_op(const char *op, int64_t operand)
{
    int64_t v;

    if (strcmp(op, "-") == 0) { v = -operand; }
    else if (strcmp(op, "+") == 0) { v = +operand; }
    else if (strcmp(op, "~") == 0) { v = ~operand; }
    else { exit(EXIT_FAILURE); }
    
    return v;
}

Expression *const_prop(Expression *expr, State *state)
{
    switch (expr->tag) {
        case (EXPR_NAME): {
            int64_t value = searchName(expr->name->name, state);
            free_name(expr->name);
            expr->tag = EXPR_INT;
            expr->int_value = create_int(value);
            return expr;
        }
        case (EXPR_INT): {
            return expr;
        }
        case (EXPR_BINARY_OP): {
            expr->binary_op->left = const_prop(expr->binary_op->left, state);
            expr->binary_op->right = const_prop(expr->binary_op->right, state);
            return expr;
        }
        case (EXPR_UNARY_OP): {
            expr->unary_op->expression = const_prop(expr->unary_op->expression, state);
            return expr;
        }
        case (EXPR_FUNCTION_CALL): {
            Arguments *args = expr->function_call->args;
            while (args != NULL)
            {
                args->arg = const_prop(args->arg, state);
                args = args->next;
            }
            return expr;
        }
        default: {
            exit(EXIT_FAILURE);
        }
    }
}

Expression *const_fold(Expression *expr, State *state)
{
    switch (expr->tag) {
        case (EXPR_NAME): {
            return const_prop(expr, state);
        }
        case (EXPR_INT): {
            return expr;
        }
        case (EXPR_BINARY_OP): {
            Expression *left = const_fold(expr->binary_op->left, state);
            Expression *right = const_fold(expr->binary_op->right, state);
            if (left->tag == EXPR_INT && right->tag == EXPR_INT) {
                int64_t value = evaluate_int_binary_op(expr->binary_op->op, left->int_value->value, right->int_value->value);
                free_binary_op(expr->binary_op);
                expr->tag = EXPR_INT;
                expr->int_value = create_int(value);
                return expr;
            }
            expr->binary_op->left = left;
            expr->binary_op->right = right;
            return expr;
        }
        case (EXPR_UNARY_OP): {
            Expression *expression = const_fold(expr->unary_op->expression, state);
            if (expr->tag == EXPR_INT) {
                int64_t value = evaluate_int_unary_op(expr->unary_op->op, expression->int_value->value);
                free_unary_op(expr->unary_op);
                expr->tag = EXPR_INT;
                expr->int_value = create_int(value);
                return expr;
            }
            expr->unary_op->expression = expression;
            return expr;
        }
        case (EXPR_FUNCTION_CALL): {
            Arguments *args = expr->function_call->args;
            while (args != NULL) {
                args->arg = const_fold(args->arg, state);
                args = args->next;
            }
            return expr;
        }
        default: {
            exit(EXIT_FAILURE);
        }
    }
}
