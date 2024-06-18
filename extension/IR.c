#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "IR.h"
#include "AST.h"

state State;

IRProgram ast_to_ir(Program program)
{
    // TODO
    // We do here the (pre, mid, post) instruction handling
    // by executing the fields in each node of the AST successively
    abort();
}

IRProgram stmts_to_ir(Statements stmts)
{
    // TODO
    // Execute a block... construct a list which we insert in the main program
    abort();
}



static int64_t *search_vars(char *name, state *State) {
    if (State->map_size > 0) {
        for(int i = 0; i < State->map_size; i++) {
            if (State->map[i].name == name) {
                return &(State->map[i].value);
            }    
        }
    }
    return NULL;
}

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

//returns pointer to the expression
static Expression *const_prop(Expression *expr, state *State) {
    switch (expr->tag)
    {
    case (EXPR_NAME):
        Int *val = malloc(sizeof(Int));
        int64_t *var = search_vars(expr->name->name, State);
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
        Expression l = *const_prop(expr->binary_op->left, State);
        Expression r = *const_prop(expr->binary_op->right, State);

        *expr->binary_op->left = l;
        *expr->binary_op->right = r;

        return expr;
    case (EXPR_UNARY_OP):
        Expression *e = const_prop(expr->unary_op->expression, State);

        expr->unary_op->expression = e;
        return expr; 
    case (EXPR_FUNCTION_CALL):
        Arguments* start = expr->function_call->args;
        while (start != NULL) {
            const_prop(start->arg,State);
            start = start->next;
        }
        return expr;
    default: //error
        return NULL;
    }
    
}


static Expression *const_fold(Expression *expr, state *State) {
    switch (expr->tag)
    {
    case (EXPR_NAME):
        return const_prop(expr, State);
    case (EXPR_INT):
        return expr;
    case (EXPR_BINARY_OP):
        Expression l = *const_fold(expr->binary_op->left, State);
        Expression r = *const_fold(expr->binary_op->right, State);
        
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
        Expression u = *const_fold(expr->unary_op->expression, State);

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
            const_fold(start->arg,State);
            start = start->next;
        }
        return expr;            
    default: // Function call 
        break;
    }
    
}

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}

