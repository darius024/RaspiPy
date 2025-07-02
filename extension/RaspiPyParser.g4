parser grammar RaspiPyParser;

options {
    superClass = RaspiPyParserBase;
    tokenVocab = RaspiPyLexer;
}

// Module
module
    : (NEWLINE | stmt)* EOF
    ;

single_input
    : NEWLINE
    | simple_stmts
    | compound_stmt NEWLINE
    ;

// Function Definition
funcdef
    : 'def' name parameters ':' block
    ;

parameters
    : '(' (name ','?)* ')'
    ;

// Statement
stmt
    : simple_stmts
    | compound_stmt
    ;

// Simple Statements
simple_stmts
    : simple_stmt (';' simple_stmt)* ';'? NEWLINE
    ;

simple_stmt
    : (
        assignment_stmt
        | flow_stmt
    )
    ;

assignment_stmt
    : expr (
        ('=' expr)?
        | assign expr
    )
    ;

assign
    : (
        '+='
        | '-='
        | '*='
        | '/='
        | '%='
        | '&='
        | '|='
        | '^='
        | '<<='
        | '>>='
    )
    ;

flow_stmt
    : break_stmt
    | continue_stmt
    | return_stmt
    ;

break_stmt
    : 'break'
    ;

continue_stmt
    : 'continue'
    ;

return_stmt
    : 'return' expr?
    ;

// Compound Statements
compound_stmt
    : if_stmt
    | while_stmt
    | for_stmt
    | funcdef
    // | match_stmt
    ;

if_stmt
    : 'if' test ':' block ('elif' test ':' block)* ('else' ':' block)?
    ;

while_stmt
    : 'while' test ':' block
    ;

for_stmt
    : 'for' expr 'in' test ':' block
    ;

// Block
block
    : simple_stmts
    | NEWLINE INDENT stmt+ DEDENT
    ;

// Tests and Logical Operations
test
    : or_test ('if' or_test 'else' test)?
    ;

or_test
    : and_test ('or' and_test)*
    ;

and_test
    : not_test ('and' not_test)*
    ;

not_test
    : 'not' not_test
    | comparison
    ;

comparison
    : expr (comp_op expr)*
    ;

comp_op
    : '<'
    | '>'
    | '=='
    | '>='
    | '<='
    | '!='
    ;

// Expression
expr
    : atom_expr
    | ('+' | '-' | '~')+ expr
    | expr ('*' | '/' | '%') expr
    | expr ('+' | '-') expr
    | expr ('<<' | '>>') expr
    | expr '&' expr
    | expr '^' expr
    | expr '|' expr
    ;

atom_expr
    : atom ('(' exprlist? ')')?
    ;

atom
    : '(' testlist? ')'
    | name
    | NUMBER
    // | STRING+
    | 'True'
    | 'False'
    ;

name
    : NAME
    ;

// Lists
exprlist
    : expr (',' expr)* ','?
    ;

testlist
    : test (',' test)* ','?
    ;
