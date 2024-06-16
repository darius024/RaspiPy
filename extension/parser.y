/*
 * RaspiPy - A python like language for easier use with Raspberry Pis
 * Parser Rules - Grammar
*/

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AST.h"
#include "lex.yy.h"

extern int yylex(void);
extern int yywrap(void);
extern void yyerror(const char *s);
extern int lineno;

%}

%union {
    int num;
    char *str;
    Program *program;
    FuncDef *func_def;
    FuncCall *func_call;
    ParamList *param_list;
    Stmts *stmts;
    Stmt *stmt;
    AssignmentStmt *assignment_stmt;
    FlowStmt *flow_stmt;
    IfStmt *if_stmt;
    WhileStmt *while_stmt;
    ForStmt *for_stmt;
    Test *test;
    BExpr *bexpr;
    Expr *expr;
    ExprList *expr_list;
    BinaryExpr *binary_expr;
    UnaryExpr *unary_expr;
    Name *name;
    Number *number;
}

%token AND BREAK CONTINUE DEF ELIF ELSE FALSE FOR GLOBAL
%token IF IN NOT OR RETURN TRUE WHILE
%token <num> DEC_INTEGER HEX_INTEGER 
%token <str> NAME
%token L_PAREN R_PAREN COMMA COLON SEMI_COLON ASSIGN NEG
%token BITWISE_OR BITWISE_XOR BITWISE_AND
%token LEFT_SHIFT RIGHT_SHIFT ADD SUB MUL DIV MOD
%token LT GT EQ GE LE NE
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token AND_ASSIGN OR_ASSIGN XOR_ASSIGN
%token LEFT_SHIFT_ASSIGN RIGHT_SHIFT_ASSIGN
%token INDENT DEDENT

%type <program> program
%type <func_def> func_def
%type <param_list> paramlist
%type <stmts> stmts
%type <stmt> stmt
%type <assignment_stmt> assignment_stmt
%type <flow_stmt> flow_stmt
%type <if_stmt> if_stmt
%type <while_stmt> while_stmt
%type <for_stmt> for_stmt
%type <test> test
%type <bexpr> bexpr
%type <expr> expr
%type <expr_list> exprlist
%type <binary_expr> binary_expr
%type <unary_expr> unary_expr
%type <name> name
%type <number> number

%start program

%%
program
    : func_defs stmts { $$ = create_program_node($1, $2); }
    ;

func_defs
    : /* empty */ { $$ = NULL; }
    | func_defs func_def { $$ = $1 ? $1 : $2; }
    ;

stmts
    : /* empty */ { $$ = NULL; }
    | stmts stmt { $$ = create_stmts_node($1, $2); }
    ;

func_def
    : DEF NAME L_PAREN paramlist R_PAREN COLON block { $$ = create_func_def_node($2, $4, $7); }
    ;

paramlist
    : paramlist COMMA NAME { $$ = create_param_list_node($1, $3); }
    | NAME { $$ = create_param_list_node(NULL, $1); }
    | /* empty */ { $$ = NULL; }
    ;

stmt
    : simple_stmt { $$ = $1; }
    | compound_stmt { $$ = $1; }
    ;

simple_stmt
    : assignment_stmt { $$ = $1; }
    | flow_stmt { $$ = $1; }
    ;

assignment_stmt
    : NAME ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME ADD_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME SUB_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME MUL_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME DIV_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME MOD_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME AND_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME OR_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME XOR_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME LEFT_SHIFT_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    | NAME RIGHT_SHIFT_ASSIGN expr { $$ = create_assignment_stmt_node($1, $3); }
    ;

flow_stmt
    : break_stmt { $$ = create_flow_stmt_node("break"); }
    | continue_stmt { $$ = create_flow_stmt_node("continue"); }
    | return_stmt { $$ = $1; }
    ;

break_stmt
    : BREAK { $$ = NULL; }
    ;

continue_stmt
    : CONTINUE { $$ = NULL; }
    ;

return_stmt
    : RETURN expr { $$ = create_flow_stmt_node("return"); $$->stmt->flow_stmt->expr = $2; }
    | RETURN { $$ = create_flow_stmt_node("return"); }
    ;

compound_stmt
    : if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | for_stmt { $$ = $1; }
    ;

if_stmt
    : IF test COLON block ELSE COLON block { $$ = create_if_stmt_node($2, $4, $7); }
    | IF test COLON block { $$ = create_if_stmt_node($2, $4, NULL); }
    ;

while_stmt
    : WHILE test COLON block { $$ = create_while_stmt_node($2, $4); }
    ;

for_stmt
    : FOR expr IN test COLON block { $$ = create_for_stmt_node($2, $4, $6); }
    ;

block
    : INDENT stmts DEDENT { $$ = $2; }
    ;

test
    : test OR and_test { $$ = create_test_node($1, $3, "or"); }
    | and_test { $$ = $1; }
    ;

and_test
    : and_test AND not_test { $$ = create_test_node($1, $3, "and"); }
    | not_test { $$ = $1; }
    ;

not_test
    : NOT not_test { $$ = create_test_node(NULL, $2, "not"); }
    | bexpr { $$ = $1; }
    | expr { $$ = $1; }
    ;

bexpr
    : expr EQ expr { $$ = create_bexpr_node($1, $3, "=="); }
    | expr NE expr { $$ = create_bexpr_node($1, $3, "!="); }
    | expr GT expr { $$ = create_bexpr_node($1, $3, ">"); }
    | expr GE expr { $$ = create_bexpr_node($1, $3, ">="); }
    | expr LT expr { $$ = create_bexpr_node($1, $3, "<"); }
    | expr LE expr { $$ = create_bexpr_node($1, $3, "<="); }
    ;

expr
    : expr ADD mult_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "+")); }
    | expr SUB mult_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "-")); }
    | mult_expr { $$ = $1; }
    ;

mult_expr
    : mult_expr MUL bitwise_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "*")); }
    | mult_expr DIV bitwise_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "/")); }
    | mult_expr MOD bitwise_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "%")); }
    | bitwise_expr { $$ = $1; }
    ;

bitwise_expr
    : bitwise_expr LEFT_SHIFT unary_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "<<")); }
    | bitwise_expr RIGHT_SHIFT unary_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, ">>")); }
    | bitwise_expr BITWISE_AND unary_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "&")); }
    | bitwise_expr BITWISE_XOR unary_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "^")); }
    | bitwise_expr BITWISE_OR unary_expr { $$ = create_expr_node(is_binary_expr, create_binary_expr_node($1, $3, "|")); }
    | unary_expr { $$ = $1; }
    ;

unary_expr
    : NEG factor { $$ = create_expr_node(is_unary_expr, create_unary_expr_node($2, "-")); }
    | SUB factor { $$ = create_expr_node(is_unary_expr, create_unary_expr_node($2, "-")); }
    | factor { $$ = $1; }
    ;

factor
    : number { $$ = $1; }
    | L_PAREN expr R_PAREN { $$ = $2; }
    | NAME L_PAREN exprlist R_PAREN { $$ = create_expr_node(is_func_call, create_func_call_node($1, $3)); }
    | NAME { $$ = create_expr_node(is_name, create_name_node($1)); }
    ;

number
    : DEC_INTEGER { $$ = create_expr_node(is_number, create_number_node($1)); }
    | HEX_INTEGER { $$ = create_expr_node(is_number, create_number_node($1)); }
    ;

exprlist
    : expr { $$ = create_expr_list_node($1, NULL); }
    | exprlist COMMA expr { $$ = create_expr_list_node($3, $1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int yywrap(void) {
    return 1;
}

