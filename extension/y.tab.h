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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define DEC_INTEGER 258
#define HEX_INTEGER 259
#define NAME 260
#define AND 261
#define BREAK 262
#define CONTINUE 263
#define DEF 264
#define ELIF 265
#define ELSE 266
#define FALSE 267
#define FOR 268
#define GLOBAL 269
#define IF 270
#define IN 271
#define NOT 272
#define OR 273
#define RETURN 274
#define TRUE 275
#define WHILE 276
#define L_PAREN 277
#define R_PAREN 278
#define COMMA 279
#define COLON 280
#define SEMI_COLON 281
#define ASSIGN 282
#define NEG 283
#define BITWISE_OR 284
#define BITWISE_XOR 285
#define BITWISE_AND 286
#define LEFT_SHIFT 287
#define RIGHT_SHIFT 288
#define ADD 289
#define SUB 290
#define MUL 291
#define DIV 292
#define MOD 293
#define LT 294
#define GT 295
#define EQ 296
#define GE 297
#define LE 298
#define NE 299
#define ADD_ASSIGN 300
#define SUB_ASSIGN 301
#define MUL_ASSIGN 302
#define DIV_ASSIGN 303
#define MOD_ASSIGN 304
#define AND_ASSIGN 305
#define OR_ASSIGN 306
#define XOR_ASSIGN 307
#define LEFT_SHIFT_ASSIGN 308
#define RIGHT_SHIFT_ASSIGN 309
#define INDENT 310
#define DEDENT 311

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "parser.y"

    int num;
    char *str;
    Program *program;
    Statements *statements;
    Statement *statement;
    AssignmentStmt *assignment_stmt;
    FlowStmt *flow_stmt;
    IfStmt *if_stmt;
    WhileStmt *while_stmt;
    ForStmt *for_stmt;
    FunctionDef *function_def;
    Parameters *parameters;
    Arguments *arguments;
    Expression *expression;
    Name *name;
    Int *int_value;
    BinaryOp *binary_op;
    UnaryOp *unary_op;
    FunctionCall *function_call;

#line 201 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
