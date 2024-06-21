/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DEC_INTEGER = 258,             /* DEC_INTEGER  */
    HEX_INTEGER = 259,             /* HEX_INTEGER  */
    NAME = 260,                    /* NAME  */
    AND = 261,                     /* AND  */
    BREAK = 262,                   /* BREAK  */
    CONTINUE = 263,                /* CONTINUE  */
    DEF = 264,                     /* DEF  */
    ELIF = 265,                    /* ELIF  */
    ELSE = 266,                    /* ELSE  */
    FALSE = 267,                   /* FALSE  */
    FOR = 268,                     /* FOR  */
    GLOBAL = 269,                  /* GLOBAL  */
    IF = 270,                      /* IF  */
    IN = 271,                      /* IN  */
    NOT = 272,                     /* NOT  */
    OR = 273,                      /* OR  */
    RETURN = 274,                  /* RETURN  */
    TRUE = 275,                    /* TRUE  */
    WHILE = 276,                   /* WHILE  */
    L_PAREN = 277,                 /* L_PAREN  */
    R_PAREN = 278,                 /* R_PAREN  */
    COMMA = 279,                   /* COMMA  */
    COLON = 280,                   /* COLON  */
    SEMI_COLON = 281,              /* SEMI_COLON  */
    ASSIGN = 282,                  /* ASSIGN  */
    NEG = 283,                     /* NEG  */
    BITWISE_OR = 284,              /* BITWISE_OR  */
    BITWISE_XOR = 285,             /* BITWISE_XOR  */
    BITWISE_AND = 286,             /* BITWISE_AND  */
    LEFT_SHIFT = 287,              /* LEFT_SHIFT  */
    RIGHT_SHIFT = 288,             /* RIGHT_SHIFT  */
    ADD = 289,                     /* ADD  */
    SUB = 290,                     /* SUB  */
    MUL = 291,                     /* MUL  */
    DIV = 292,                     /* DIV  */
    MOD = 293,                     /* MOD  */
    LT = 294,                      /* LT  */
    GT = 295,                      /* GT  */
    EQ = 296,                      /* EQ  */
    GE = 297,                      /* GE  */
    LE = 298,                      /* LE  */
    NE = 299,                      /* NE  */
    ADD_ASSIGN = 300,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 301,              /* SUB_ASSIGN  */
    MUL_ASSIGN = 302,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 303,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 304,              /* MOD_ASSIGN  */
    AND_ASSIGN = 305,              /* AND_ASSIGN  */
    OR_ASSIGN = 306,               /* OR_ASSIGN  */
    XOR_ASSIGN = 307,              /* XOR_ASSIGN  */
    LEFT_SHIFT_ASSIGN = 308,       /* LEFT_SHIFT_ASSIGN  */
    RIGHT_SHIFT_ASSIGN = 309,      /* RIGHT_SHIFT_ASSIGN  */
    INDENT = 310,                  /* INDENT  */
    DEDENT = 311                   /* DEDENT  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 36 "parser.y"

    int num;
    char *str;
    struct Program *program;
    struct Statements *statements;
    struct Statement *statement;
    struct AssignmentStmt *assignment_stmt;
    struct FlowStmt *flow_stmt;
    struct IfStmt *if_stmt;
    struct WhileStmt *while_stmt;
    struct ForStmt *for_stmt;
    struct FunctionDef *function_def;
    struct Parameters *parameters;
    struct Arguments *arguments;
    struct Expression *expression;
    struct Name *name;
    struct Int *int_value;
    struct BinaryOp *binary_op;
    struct UnaryOp *unary_op;
    struct FunctionCall *function_call;

#line 142 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */
