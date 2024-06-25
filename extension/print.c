#include "print.h"
#include "ast.h"
#include "ir.h"
#include "ast_to_ir.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INT_LENGTH 20
#define MAX_TOKEN_LENGTH 20
#define INDENT 2
#define INITIAL_INDENT 0


/*
 * Print IR / Assembly
*/
static void print_branch_condition(Token *token, FILE *output)
{
    switch(token->value) {
        case B_EQ: fprintf(output, "eq"); break;
        case B_NE: fprintf(output, "ne"); break;
        case B_GE: fprintf(output, "ge"); break;
        case B_GT: fprintf(output, "gt"); break;
        case B_LE: fprintf(output, "le"); break;
        case B_LT: fprintf(output, "lt"); break;
        default: perror("Invalid conditional branch.\n"); exit(EXIT_FAILURE);
    }
}

static void print_directives(State *state, FILE *output)
{
    for (int i = 0; i < state->directives_size; i++) {
        int first_half = state->directives[i]->value >> 32;
        int second_half = state->directives[i]->value;

        fprintf(output, "%s:\n", state->directives[i]->name);
        fprintf(output, ".int 0x%x\n", second_half);
        fprintf(output, ".int 0x%x\n", first_half);
    }
}

static void print_label(State *state, int64_t address, FILE *output)
{
    for (int i = 0; i < state->symbol_table_size; i++) {
        if (state->symbol_table[i]->address == address) {
            if (!strcmp(state->symbol_table[i]->name, "main")) fprintf(output, "\n");
            fprintf(output, "%s:\n", state->symbol_table[i]->name);
        }
    }
}

static void print_empty_labels(State *state, IRInstruction *tail, FILE *output)
{
    for (int i = 0; i < state->symbol_table_size; i++) {
        if (state->symbol_table[i]->address > tail->line) {
            fprintf(output, "%s:\n", state->symbol_table[i]->name);
        }
    }
}

static void print_mnemonic(IRType type, FILE *output)
{
    switch (type) {
        case IR_ADD: fprintf(output, "add"); break;
        case IR_SUB: fprintf(output, "sub"); break;
        case IR_ADDS: fprintf(output, "adds"); break;
        case IR_SUBS: fprintf(output, "subs"); break;
        case IR_CMP: fprintf(output, "cmp"); break;
        case IR_NEG: fprintf(output, "neg"); break;
        case IR_AND: fprintf(output, "and"); break;
        case IR_EOR: fprintf(output, "eor"); break;
        case IR_ORR: fprintf(output, "orr"); break;
        case IR_TST: fprintf(output, "tst"); break;
        case IR_MOV: fprintf(output, "mov"); break;
        case IR_MOVZ: fprintf(output, "movz"); break;
        case IR_MVN: fprintf(output, "mvn"); break;
        case IR_MADD: fprintf(output, "madd"); break;
        case IR_MUL: fprintf(output, "mul"); break;
        case IR_B: fprintf(output, "b"); break;
        case IR_BR: fprintf(output, "br"); break;
        case IR_BCOND: fprintf(output, "b."); break;
        case IR_LDR: fprintf(output, "ldr"); break;
        case IR_STR: fprintf(output, "str"); break;
        case IR_DIR: fprintf(output, ".int"); break;
        default: perror("Print mnemonic.\n"); exit(EXIT_FAILURE);
    }
}

void print_token(State *state, Token *token, FILE *output)
{
    switch(token->type) {
        case IMM: fprintf(output, "#%d", token->value); break;
        case REGX:
            if (token->value != 32) fprintf(output, "x%d", token->value);
            else fprintf(output, "sp");
            break;
        case REGW:
            if (token->value != 32) fprintf(output, "w%d", token->value);
            else fprintf(output, "sp");
            break;
        case BC: break;
        case DIR: fprintf(output, "%s", state->directives[token->value]->name); break;
        case LABEL: fprintf(output, "%s", state->symbol_table[token->value]->name); break;
        default: perror("Print token.\n"); exit(EXIT_FAILURE);
    }
}

void print_ir_instruction(IRInstruction *instr, State *state, FILE *output)
{
    if (instr) {
        print_label(state, instr->line, output);
        fprintf(output, "  ");
        print_mnemonic(instr->type, output);
        if (instr->dest != NULL) {
            if (instr->dest->type == BC) {
                print_branch_condition(instr->dest, output);
            } else {
                fprintf(output, " ");
                print_token(state, instr->dest, output);
            }
        }
        if (instr->src1 != NULL) {
            if (instr->dest->type != BC) fprintf(output, ",");
            fprintf(output, " ");
            if ((instr->type == IR_STR || instr->type == IR_LDR) && instr->src1->type == REGX) fprintf(output, "[");
            print_token(state, instr->src1, output);
            if ((instr->type == IR_STR || instr->type == IR_LDR) && instr->src1->type == REGX) fprintf(output, "]");
        }
        if (instr->src2 != NULL) {
            fprintf(output, ", ");
            print_token(state, instr->src2, output);
        }
        if (instr->src3 != NULL) {
            fprintf(output, ", ");
            print_token(state, instr->src3, output);
        }
        fprintf(output, "\n");
    } else {
        printf("NULL\n");
    }
}

void print_ir_program(IRProgram *program, State *state, FILE *output)
{
    if (program) {
        // Branch to the main program
        IRInstruction *current = program->head;
        print_ir_instruction(current, state, output);
        current = current->next;

        // Get "main" address
        int main_address = 0;
        for (int i = 0; i< state->symbol_table_size; i++) {
            if(!strcmp(state->symbol_table[i]->name, "main")) {
                main_address = state->symbol_table[i]->address;
                break;
            }
        }
        // Print function definitions with their own state
        for (int i = 0; i < state->funcs_size; i++) {
            State *func_state = state->funcs[i]->state;
            int64_t stop_line = (i == state->funcs_size - 1) ? main_address : state->funcs[i + 1]->address;
            while (i < state->funcs_size && current->line < stop_line) {
                print_ir_instruction(current, func_state, output);
                current = current->next;
            }
            print_empty_labels(func_state, current, output);
        }

        // Print global code and main functions
        while (current) {
            print_ir_instruction(current, state, output);
            current = current->next;
        }
        print_empty_labels(state, program->tail, output);
        
        fprintf(output, "\n\n");

        print_directives(state, output);
        for (int i = 0; i < state->funcs_size; i++) {
            print_directives(state->funcs[i]->state, output);
        }
        // print_variables(state);
    } else {
        printf("NULL\n");
    }
}


/*
 * Print AST
*/

static void print_indentation(int indent)
{
    for(int i = 0; i < indent; i++) {
        printf(" ");
    }
}


static void print_expression(Expression *expr)
{
    if (expr == NULL) return;
    switch(expr->tag) {
        case EXPR_NAME:
            printf("%s", expr->name->name);
            break;
        case EXPR_INT: {
            printf("%ld", expr->int_value->value);
            break;
        }
        case EXPR_BINARY_OP: {
            BinaryOp *b = expr->binary_op;
            printf("(");
            print_expression(b->left);
            printf(" %s ", b->op);
            print_expression(b->right);
            printf(")");
            break;
        }
        case EXPR_UNARY_OP: {
            UnaryOp *u = expr->unary_op;
            printf("%s", u->op);
            printf("(");
            print_expression(u->expression);
            printf(")");
            break;
        }
        case EXPR_FUNCTION_CALL: {
            FunctionCall *fc = expr->function_call;
            printf("%s(", fc->name);
            for (Arguments *args = fc->args ; args != NULL ; args = args->next) {
                print_expression(args->arg);
                if (args->next != NULL) printf(", ");
            }
            printf(")");
            break;
        }
    }
}

void print_ast(Statements *statements, int indent)
{
    if (statements == NULL) {
        return;
    }
    print_indentation(indent);
    switch(statements->statement->tag) {
        case ASSIGNMENT_STMT: {
            AssignmentStmt *assignment_stmt = statements->statement->assignment_stmt;
            printf("%s = ", assignment_stmt->name);
            print_expression(assignment_stmt->expression);
            printf("\n");
            break;
        }
        case FLOW_STMT: {
            FlowStmt *flow_stmt = statements->statement->flow_stmt;
            printf("%s ", flow_stmt->name);
            print_expression(flow_stmt->expression);
            printf("\n");
            break;
        }
        case IF_STMT: {
            IfStmt *if_stmt = statements->statement->if_stmt;
            printf("if ");
            print_expression(if_stmt->condition);
            printf(":\n");
            print_ast(if_stmt->then_block, indent + INDENT);
            if (if_stmt->else_block != NULL) {
                print_indentation(indent);
                printf("else:\n");
                print_ast(if_stmt->else_block, indent + INDENT);
            }
            break;
        }
            
        case WHILE_STMT: {
            WhileStmt *while_stmt = statements->statement->while_stmt;
            printf("while ");
            print_expression(while_stmt->condition);
            printf(":\n");
            print_ast(while_stmt->block, indent + INDENT);
            break;
        }
        
        case FOR_STMT: {
            ForStmt *for_stmt = statements->statement->for_stmt;
            printf("for %s in ", for_stmt->var);
            print_expression(for_stmt->range);
            printf(":\n");
            print_indentation(INDENT);
            print_ast(for_stmt->block, indent + INDENT);
            break;
        }
        case FUNCTION_DEF: {
            FunctionDef *function_def = statements->statement->function_def;
            printf("def %s(", function_def->name);
            for (Parameters *current = function_def->parameters ; current != NULL ; current = current->next) {
                if (current->next != NULL) {
                    printf("%s, ", current->parameter->name);
                } else {
                    printf("%s", current->parameter->name);
                }
            }
            printf("):\n");
            print_ast(function_def->body, indent + INDENT);
        }
        default: {
            break;
        }
    }
    print_ast(statements->next, indent);
}

void print_program(Program *program)
{
    if (program != NULL) {
        print_ast(program->statements, INITIAL_INDENT);
    }
}
