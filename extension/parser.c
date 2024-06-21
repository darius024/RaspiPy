/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 12 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"


extern int yylex(void);

Program *program = NULL;
int yyerrors = 0;

void yyerror(const char *str) {
    //fprintf(stderr, "Error on line %d: %s\n", yylineno, str);
    yyerrors++;
}

int yywrap(void) {
    return 1;
}


#line 95 "parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DEC_INTEGER = 3,                /* DEC_INTEGER  */
  YYSYMBOL_HEX_INTEGER = 4,                /* HEX_INTEGER  */
  YYSYMBOL_NAME = 5,                       /* NAME  */
  YYSYMBOL_AND = 6,                        /* AND  */
  YYSYMBOL_BREAK = 7,                      /* BREAK  */
  YYSYMBOL_CONTINUE = 8,                   /* CONTINUE  */
  YYSYMBOL_DEF = 9,                        /* DEF  */
  YYSYMBOL_ELIF = 10,                      /* ELIF  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_FALSE = 12,                     /* FALSE  */
  YYSYMBOL_FOR = 13,                       /* FOR  */
  YYSYMBOL_GLOBAL = 14,                    /* GLOBAL  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_IN = 16,                        /* IN  */
  YYSYMBOL_NOT = 17,                       /* NOT  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_RETURN = 19,                    /* RETURN  */
  YYSYMBOL_TRUE = 20,                      /* TRUE  */
  YYSYMBOL_WHILE = 21,                     /* WHILE  */
  YYSYMBOL_L_PAREN = 22,                   /* L_PAREN  */
  YYSYMBOL_R_PAREN = 23,                   /* R_PAREN  */
  YYSYMBOL_COMMA = 24,                     /* COMMA  */
  YYSYMBOL_COLON = 25,                     /* COLON  */
  YYSYMBOL_SEMI_COLON = 26,                /* SEMI_COLON  */
  YYSYMBOL_ASSIGN = 27,                    /* ASSIGN  */
  YYSYMBOL_NEG = 28,                       /* NEG  */
  YYSYMBOL_BITWISE_OR = 29,                /* BITWISE_OR  */
  YYSYMBOL_BITWISE_XOR = 30,               /* BITWISE_XOR  */
  YYSYMBOL_BITWISE_AND = 31,               /* BITWISE_AND  */
  YYSYMBOL_LEFT_SHIFT = 32,                /* LEFT_SHIFT  */
  YYSYMBOL_RIGHT_SHIFT = 33,               /* RIGHT_SHIFT  */
  YYSYMBOL_ADD = 34,                       /* ADD  */
  YYSYMBOL_SUB = 35,                       /* SUB  */
  YYSYMBOL_MUL = 36,                       /* MUL  */
  YYSYMBOL_DIV = 37,                       /* DIV  */
  YYSYMBOL_MOD = 38,                       /* MOD  */
  YYSYMBOL_LT = 39,                        /* LT  */
  YYSYMBOL_GT = 40,                        /* GT  */
  YYSYMBOL_EQ = 41,                        /* EQ  */
  YYSYMBOL_GE = 42,                        /* GE  */
  YYSYMBOL_LE = 43,                        /* LE  */
  YYSYMBOL_NE = 44,                        /* NE  */
  YYSYMBOL_ADD_ASSIGN = 45,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 46,                /* SUB_ASSIGN  */
  YYSYMBOL_MUL_ASSIGN = 47,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 48,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 49,                /* MOD_ASSIGN  */
  YYSYMBOL_AND_ASSIGN = 50,                /* AND_ASSIGN  */
  YYSYMBOL_OR_ASSIGN = 51,                 /* OR_ASSIGN  */
  YYSYMBOL_XOR_ASSIGN = 52,                /* XOR_ASSIGN  */
  YYSYMBOL_LEFT_SHIFT_ASSIGN = 53,         /* LEFT_SHIFT_ASSIGN  */
  YYSYMBOL_RIGHT_SHIFT_ASSIGN = 54,        /* RIGHT_SHIFT_ASSIGN  */
  YYSYMBOL_INDENT = 55,                    /* INDENT  */
  YYSYMBOL_DEDENT = 56,                    /* DEDENT  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_program = 58,                   /* program  */
  YYSYMBOL_statements = 59,                /* statements  */
  YYSYMBOL_statement = 60,                 /* statement  */
  YYSYMBOL_simple_stmt = 61,               /* simple_stmt  */
  YYSYMBOL_assignment_stmt = 62,           /* assignment_stmt  */
  YYSYMBOL_return_stmt = 63,               /* return_stmt  */
  YYSYMBOL_compound_stmt = 64,             /* compound_stmt  */
  YYSYMBOL_if_stmt = 65,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 66,                /* while_stmt  */
  YYSYMBOL_for_stmt = 67,                  /* for_stmt  */
  YYSYMBOL_function_def = 68,              /* function_def  */
  YYSYMBOL_parameters = 69,                /* parameters  */
  YYSYMBOL_block = 70,                     /* block  */
  YYSYMBOL_expression = 71,                /* expression  */
  YYSYMBOL_disjunction = 72,               /* disjunction  */
  YYSYMBOL_conjunction = 73,               /* conjunction  */
  YYSYMBOL_inversion = 74,                 /* inversion  */
  YYSYMBOL_comparison = 75,                /* comparison  */
  YYSYMBOL_bitwise_or = 76,                /* bitwise_or  */
  YYSYMBOL_bitwise_xor = 77,               /* bitwise_xor  */
  YYSYMBOL_bitwise_and = 78,               /* bitwise_and  */
  YYSYMBOL_shift_expr = 79,                /* shift_expr  */
  YYSYMBOL_sum = 80,                       /* sum  */
  YYSYMBOL_term = 81,                      /* term  */
  YYSYMBOL_factor = 82,                    /* factor  */
  YYSYMBOL_function_call = 83,             /* function_call  */
  YYSYMBOL_arguments = 84,                 /* arguments  */
  YYSYMBOL_atom = 85                       /* atom  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  137

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   311


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    94,    94,    98,    99,   103,   104,   108,   109,   110,
     111,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   129,   130,   134,   135,   136,   137,   141,   142,
     146,   150,   154,   158,   159,   160,   164,   168,   172,   173,
     177,   178,   182,   183,   187,   188,   189,   190,   191,   192,
     193,   197,   198,   202,   203,   207,   208,   212,   213,   214,
     218,   219,   220,   224,   225,   226,   227,   231,   232,   233,
     234,   235,   239,   243,   244,   245,   249,   250,   251,   252
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DEC_INTEGER",
  "HEX_INTEGER", "NAME", "AND", "BREAK", "CONTINUE", "DEF", "ELIF", "ELSE",
  "FALSE", "FOR", "GLOBAL", "IF", "IN", "NOT", "OR", "RETURN", "TRUE",
  "WHILE", "L_PAREN", "R_PAREN", "COMMA", "COLON", "SEMI_COLON", "ASSIGN",
  "NEG", "BITWISE_OR", "BITWISE_XOR", "BITWISE_AND", "LEFT_SHIFT",
  "RIGHT_SHIFT", "ADD", "SUB", "MUL", "DIV", "MOD", "LT", "GT", "EQ", "GE",
  "LE", "NE", "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN",
  "MOD_ASSIGN", "AND_ASSIGN", "OR_ASSIGN", "XOR_ASSIGN",
  "LEFT_SHIFT_ASSIGN", "RIGHT_SHIFT_ASSIGN", "INDENT", "DEDENT", "$accept",
  "program", "statements", "statement", "simple_stmt", "assignment_stmt",
  "return_stmt", "compound_stmt", "if_stmt", "while_stmt", "for_stmt",
  "function_def", "parameters", "block", "expression", "disjunction",
  "conjunction", "inversion", "comparison", "bitwise_or", "bitwise_xor",
  "bitwise_and", "shift_expr", "sum", "term", "factor", "function_call",
  "arguments", "atom", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-91)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -91,    10,    68,   -91,    18,   -91,   -91,    17,    22,    20,
      20,    20,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,    20,    20,    20,    20,    20,    20,    20,    20,    20,
      20,    20,    13,    25,   -91,   -91,    21,    20,    20,     4,
       4,     4,    24,    28,    53,   -91,   -91,    51,    32,    43,
       1,    16,    -7,   -91,   -91,   -91,   -91,    57,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,    81,
      20,    20,   -91,    65,   -91,   -91,   -91,   -91,    20,    20,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,   -91,   -91,    29,    79,
     -91,    55,   -91,    68,    94,    53,   -91,    32,    77,    77,
      77,    77,    77,    77,    43,     1,    16,    16,    -7,    -7,
     -91,   -91,   -91,   -91,    82,   103,   -91,   -91,    20,    84,
     -91,   -91,   -91,   -91,   -91,   -91,   -91
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,     9,    10,     0,     0,     0,
      23,     0,     4,     6,     7,     8,     5,    24,    25,    26,
      27,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    77,    78,    76,     0,     0,     0,
       0,     0,     0,    37,    39,    41,    43,    50,    52,    54,
      56,    59,    62,    66,    70,    71,    22,     0,    11,    13,
      12,    14,    15,    16,    17,    18,    19,    20,    21,    35,
       0,    75,    42,     0,    69,    67,    68,     3,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     3,    34,     0,     0,
      74,     0,    79,    36,    29,    38,    40,    51,    48,    46,
      44,    47,    49,    45,    53,    55,    57,    58,    60,    61,
      63,    64,    65,    30,     0,     0,     3,    72,     0,     0,
       3,    33,    31,    73,     3,    32,    28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -91,   -91,   110,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -90,   -10,   -91,    33,   -32,   -91,    15,
      34,    26,    27,    -5,    11,   -37,   -91,   -91,   -91
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,   103,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    98,   104,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,   101,    55
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      56,    57,    74,    75,    76,    72,   123,    34,    35,    36,
       3,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    32,    34,    35,    36,    38,    33,    73,    93,
      94,    95,    39,    89,    90,    69,   132,    37,    40,    41,
     135,    70,    38,    71,   136,    21,    78,   106,    39,    77,
      91,    92,   124,   125,    40,    41,   120,   121,   122,    79,
      99,   100,    87,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,     4,    88,     5,     6,     7,   127,   128,
      80,     8,    96,     9,   116,   117,    97,    10,   102,    11,
      81,    82,    83,    84,    85,    86,   108,   109,   110,   111,
     112,   113,   118,   119,   126,   129,    80,   130,   131,   134,
       2,   105,     0,   114,   107,   115,     0,     0,   133
};

static const yytype_int16 yycheck[] =
{
      10,    11,    39,    40,    41,    37,    96,     3,     4,     5,
       0,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,     5,     3,     4,     5,    22,     5,    38,    36,
      37,    38,    28,    32,    33,    22,   126,    17,    34,    35,
     130,    16,    22,    22,   134,    27,    18,    79,    28,    25,
      34,    35,    23,    24,    34,    35,    93,    94,    95,     6,
      70,    71,    30,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,     5,    31,     7,     8,     9,    23,    24,
      29,    13,    25,    15,    89,    90,     5,    19,    23,    21,
      39,    40,    41,    42,    43,    44,    81,    82,    83,    84,
      85,    86,    91,    92,    25,    11,    29,    25,     5,    25,
       0,    78,    -1,    87,    80,    88,    -1,    -1,   128
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    58,    59,     0,     5,     7,     8,     9,    13,    15,
      19,    21,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    27,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,     5,     5,     3,     4,     5,    17,    22,    28,
      34,    35,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    85,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    22,
      16,    22,    74,    71,    82,    82,    82,    25,    18,     6,
      29,    39,    40,    41,    42,    43,    44,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    25,     5,    69,    71,
      71,    84,    23,    59,    70,    73,    74,    77,    76,    76,
      76,    76,    76,    76,    78,    79,    80,    80,    81,    81,
      82,    82,    82,    70,    23,    24,    25,    23,    24,    11,
      25,     5,    70,    71,    25,    70,    70
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    59,    59,    60,    60,    61,    61,    61,
      61,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    63,    63,    64,    64,    64,    64,    65,    65,
      66,    67,    68,    69,    69,    69,    70,    71,    72,    72,
      73,    73,    74,    74,    75,    75,    75,    75,    75,    75,
      75,    76,    76,    77,    77,    78,    78,    79,    79,    79,
      80,    80,    80,    81,    81,    81,    81,    82,    82,    82,
      82,    82,    83,    84,    84,    84,    85,    85,    85,    85
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     1,     1,     1,     7,     4,
       4,     6,     7,     3,     1,     0,     1,     1,     3,     1,
       3,     1,     2,     1,     3,     3,     3,     3,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     2,     2,     2,
       1,     1,     4,     3,     1,     0,     1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: statements  */
#line 94 "parser.y"
                 { program = create_program((yyvsp[0].statements)); }
#line 1258 "parser.c"
    break;

  case 3: /* statements: %empty  */
#line 98 "parser.y"
                  { (yyval.statements) = NULL; }
#line 1264 "parser.c"
    break;

  case 4: /* statements: statements statement  */
#line 99 "parser.y"
                           { (yyval.statements) = append_statements((yyvsp[-1].statements), (yyvsp[0].statement)); }
#line 1270 "parser.c"
    break;

  case 5: /* statement: compound_stmt  */
#line 103 "parser.y"
                    { (yyval.statement) = (yyvsp[0].statement); }
#line 1276 "parser.c"
    break;

  case 6: /* statement: simple_stmt  */
#line 104 "parser.y"
                  { (yyval.statement) = (yyvsp[0].statement); }
#line 1282 "parser.c"
    break;

  case 7: /* simple_stmt: assignment_stmt  */
#line 108 "parser.y"
                      { (yyval.statement) = create_statement(ASSIGNMENT_STMT, (yyvsp[0].assignment_stmt)); }
#line 1288 "parser.c"
    break;

  case 8: /* simple_stmt: return_stmt  */
#line 109 "parser.y"
                  { (yyval.statement) = create_statement(FLOW_STMT, (yyvsp[0].flow_stmt)); }
#line 1294 "parser.c"
    break;

  case 9: /* simple_stmt: BREAK  */
#line 110 "parser.y"
            { (yyval.statement) = create_statement(FLOW_STMT, create_flow_stmt("break", NULL)); }
#line 1300 "parser.c"
    break;

  case 10: /* simple_stmt: CONTINUE  */
#line 111 "parser.y"
               { (yyval.statement) = create_statement(FLOW_STMT, create_flow_stmt("continue", NULL)); }
#line 1306 "parser.c"
    break;

  case 11: /* assignment_stmt: NAME ASSIGN expression  */
#line 115 "parser.y"
                             { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), (yyvsp[0].expression)); }
#line 1312 "parser.c"
    break;

  case 12: /* assignment_stmt: NAME SUB_ASSIGN expression  */
#line 116 "parser.y"
                                 { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("-=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1318 "parser.c"
    break;

  case 13: /* assignment_stmt: NAME ADD_ASSIGN expression  */
#line 117 "parser.y"
                                 { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("+=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1324 "parser.c"
    break;

  case 14: /* assignment_stmt: NAME MUL_ASSIGN expression  */
#line 118 "parser.y"
                                 { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("*=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1330 "parser.c"
    break;

  case 15: /* assignment_stmt: NAME DIV_ASSIGN expression  */
#line 119 "parser.y"
                                 { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("/=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1336 "parser.c"
    break;

  case 16: /* assignment_stmt: NAME MOD_ASSIGN expression  */
#line 120 "parser.y"
                                 { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("%=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1342 "parser.c"
    break;

  case 17: /* assignment_stmt: NAME AND_ASSIGN expression  */
#line 121 "parser.y"
                                 { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("&=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1348 "parser.c"
    break;

  case 18: /* assignment_stmt: NAME OR_ASSIGN expression  */
#line 122 "parser.y"
                                { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("|=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1354 "parser.c"
    break;

  case 19: /* assignment_stmt: NAME XOR_ASSIGN expression  */
#line 123 "parser.y"
                                 { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("^=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1360 "parser.c"
    break;

  case 20: /* assignment_stmt: NAME LEFT_SHIFT_ASSIGN expression  */
#line 124 "parser.y"
                                        { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op("<<=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1366 "parser.c"
    break;

  case 21: /* assignment_stmt: NAME RIGHT_SHIFT_ASSIGN expression  */
#line 125 "parser.y"
                                         { (yyval.assignment_stmt) = create_assignment_stmt((yyvsp[-2].str), create_expression(EXPR_BINARY_OP, create_binary_op(">>=", create_expression(EXPR_NAME, create_name((yyvsp[-2].str))), (yyvsp[0].expression)))); }
#line 1372 "parser.c"
    break;

  case 22: /* return_stmt: RETURN expression  */
#line 129 "parser.y"
                        { (yyval.flow_stmt) = create_flow_stmt("return", (yyvsp[0].expression)); }
#line 1378 "parser.c"
    break;

  case 23: /* return_stmt: RETURN  */
#line 130 "parser.y"
             { (yyval.flow_stmt) = create_flow_stmt("return", NULL); }
#line 1384 "parser.c"
    break;

  case 24: /* compound_stmt: if_stmt  */
#line 134 "parser.y"
              { (yyval.statement) = create_statement(IF_STMT, (yyvsp[0].if_stmt)); }
#line 1390 "parser.c"
    break;

  case 25: /* compound_stmt: while_stmt  */
#line 135 "parser.y"
                 { (yyval.statement) = create_statement(WHILE_STMT, (yyvsp[0].while_stmt)); }
#line 1396 "parser.c"
    break;

  case 26: /* compound_stmt: for_stmt  */
#line 136 "parser.y"
               { (yyval.statement) = create_statement(FOR_STMT, (yyvsp[0].for_stmt)); }
#line 1402 "parser.c"
    break;

  case 27: /* compound_stmt: function_def  */
#line 137 "parser.y"
                   { (yyval.statement) = create_statement(FUNCTION_DEF, (yyvsp[0].function_def)); }
#line 1408 "parser.c"
    break;

  case 28: /* if_stmt: IF expression COLON block ELSE COLON block  */
#line 141 "parser.y"
                                                 { (yyval.if_stmt) = create_if_stmt((yyvsp[-5].expression), (yyvsp[-3].statements), (yyvsp[0].statements)); }
#line 1414 "parser.c"
    break;

  case 29: /* if_stmt: IF expression COLON block  */
#line 142 "parser.y"
                                { (yyval.if_stmt) = create_if_stmt((yyvsp[-2].expression), (yyvsp[0].statements), NULL); }
#line 1420 "parser.c"
    break;

  case 30: /* while_stmt: WHILE expression COLON block  */
#line 146 "parser.y"
                                   { (yyval.while_stmt) = create_while_stmt((yyvsp[-2].expression), (yyvsp[0].statements)); }
#line 1426 "parser.c"
    break;

  case 31: /* for_stmt: FOR NAME IN expression COLON block  */
#line 150 "parser.y"
                                         { (yyval.for_stmt) = create_for_stmt((yyvsp[-4].str), (yyvsp[-2].expression), (yyvsp[0].statements)); }
#line 1432 "parser.c"
    break;

  case 32: /* function_def: DEF NAME L_PAREN parameters R_PAREN COLON block  */
#line 154 "parser.y"
                                                      { (yyval.function_def) = create_function_def((yyvsp[-5].str), (yyvsp[-3].parameters), (yyvsp[0].statements)); }
#line 1438 "parser.c"
    break;

  case 33: /* parameters: parameters COMMA NAME  */
#line 158 "parser.y"
                            { (yyval.parameters) = append_parameters((yyvsp[-2].parameters), create_name((yyvsp[0].str))); }
#line 1444 "parser.c"
    break;

  case 34: /* parameters: NAME  */
#line 159 "parser.y"
           { (yyval.parameters) = create_parameters(create_name((yyvsp[0].str))); }
#line 1450 "parser.c"
    break;

  case 35: /* parameters: %empty  */
#line 160 "parser.y"
                  { (yyval.parameters) = NULL; }
#line 1456 "parser.c"
    break;

  case 36: /* block: statements  */
#line 164 "parser.y"
                 { (yyval.statements) = (yyvsp[0].statements); }
#line 1462 "parser.c"
    break;

  case 37: /* expression: disjunction  */
#line 168 "parser.y"
                  { (yyval.expression) = (yyvsp[0].expression); }
#line 1468 "parser.c"
    break;

  case 38: /* disjunction: disjunction OR conjunction  */
#line 172 "parser.y"
                                 { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("or", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1474 "parser.c"
    break;

  case 39: /* disjunction: conjunction  */
#line 173 "parser.y"
                  { (yyval.expression) = (yyvsp[0].expression); }
#line 1480 "parser.c"
    break;

  case 40: /* conjunction: conjunction AND inversion  */
#line 177 "parser.y"
                                { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("and", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1486 "parser.c"
    break;

  case 41: /* conjunction: inversion  */
#line 178 "parser.y"
                { (yyval.expression) = (yyvsp[0].expression); }
#line 1492 "parser.c"
    break;

  case 42: /* inversion: NOT inversion  */
#line 182 "parser.y"
                    { (yyval.expression) = create_expression(EXPR_UNARY_OP, create_unary_op("not", (yyvsp[0].expression))); }
#line 1498 "parser.c"
    break;

  case 43: /* inversion: comparison  */
#line 183 "parser.y"
                 { (yyval.expression) = (yyvsp[0].expression); }
#line 1504 "parser.c"
    break;

  case 44: /* comparison: bitwise_or EQ bitwise_or  */
#line 187 "parser.y"
                               { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("==", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1510 "parser.c"
    break;

  case 45: /* comparison: bitwise_or NE bitwise_or  */
#line 188 "parser.y"
                               { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("!=", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1516 "parser.c"
    break;

  case 46: /* comparison: bitwise_or GT bitwise_or  */
#line 189 "parser.y"
                               { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op(">", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1522 "parser.c"
    break;

  case 47: /* comparison: bitwise_or GE bitwise_or  */
#line 190 "parser.y"
                               { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op(">=", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1528 "parser.c"
    break;

  case 48: /* comparison: bitwise_or LT bitwise_or  */
#line 191 "parser.y"
                               { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("<", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1534 "parser.c"
    break;

  case 49: /* comparison: bitwise_or LE bitwise_or  */
#line 192 "parser.y"
                               { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("<=", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1540 "parser.c"
    break;

  case 50: /* comparison: bitwise_or  */
#line 193 "parser.y"
                 { (yyval.expression) = (yyvsp[0].expression); }
#line 1546 "parser.c"
    break;

  case 51: /* bitwise_or: bitwise_or BITWISE_OR bitwise_xor  */
#line 197 "parser.y"
                                        { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("|", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1552 "parser.c"
    break;

  case 52: /* bitwise_or: bitwise_xor  */
#line 198 "parser.y"
                  { (yyval.expression) = (yyvsp[0].expression); }
#line 1558 "parser.c"
    break;

  case 53: /* bitwise_xor: bitwise_xor BITWISE_XOR bitwise_and  */
#line 202 "parser.y"
                                          { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("^", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1564 "parser.c"
    break;

  case 54: /* bitwise_xor: bitwise_and  */
#line 203 "parser.y"
                  { (yyval.expression) = (yyvsp[0].expression); }
#line 1570 "parser.c"
    break;

  case 55: /* bitwise_and: bitwise_and BITWISE_AND shift_expr  */
#line 207 "parser.y"
                                         { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("&", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1576 "parser.c"
    break;

  case 56: /* bitwise_and: shift_expr  */
#line 208 "parser.y"
                 { (yyval.expression) = (yyvsp[0].expression); }
#line 1582 "parser.c"
    break;

  case 57: /* shift_expr: shift_expr LEFT_SHIFT sum  */
#line 212 "parser.y"
                                { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("<<", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1588 "parser.c"
    break;

  case 58: /* shift_expr: shift_expr RIGHT_SHIFT sum  */
#line 213 "parser.y"
                                 { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op(">>", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1594 "parser.c"
    break;

  case 59: /* shift_expr: sum  */
#line 214 "parser.y"
          { (yyval.expression) = (yyvsp[0].expression); }
#line 1600 "parser.c"
    break;

  case 60: /* sum: sum ADD term  */
#line 218 "parser.y"
                   { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("+", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1606 "parser.c"
    break;

  case 61: /* sum: sum SUB term  */
#line 219 "parser.y"
                   { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("-", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1612 "parser.c"
    break;

  case 62: /* sum: term  */
#line 220 "parser.y"
           { (yyval.expression) = (yyvsp[0].expression); }
#line 1618 "parser.c"
    break;

  case 63: /* term: term MUL factor  */
#line 224 "parser.y"
                      { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("*", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1624 "parser.c"
    break;

  case 64: /* term: term DIV factor  */
#line 225 "parser.y"
                      { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("/", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1630 "parser.c"
    break;

  case 65: /* term: term MOD factor  */
#line 226 "parser.y"
                      { (yyval.expression) = create_expression(EXPR_BINARY_OP, create_binary_op("%", (yyvsp[-2].expression), (yyvsp[0].expression))); }
#line 1636 "parser.c"
    break;

  case 66: /* term: factor  */
#line 227 "parser.y"
             { (yyval.expression) = (yyvsp[0].expression); }
#line 1642 "parser.c"
    break;

  case 67: /* factor: ADD factor  */
#line 231 "parser.y"
                 { (yyval.expression) = create_expression(EXPR_UNARY_OP, create_unary_op("+", (yyvsp[0].expression))); }
#line 1648 "parser.c"
    break;

  case 68: /* factor: SUB factor  */
#line 232 "parser.y"
                 { (yyval.expression) = create_expression(EXPR_UNARY_OP, create_unary_op("-", (yyvsp[0].expression))); }
#line 1654 "parser.c"
    break;

  case 69: /* factor: NEG factor  */
#line 233 "parser.y"
                 { (yyval.expression) = create_expression(EXPR_UNARY_OP, create_unary_op("~", (yyvsp[0].expression))); }
#line 1660 "parser.c"
    break;

  case 70: /* factor: function_call  */
#line 234 "parser.y"
                    { (yyval.expression) = (yyvsp[0].expression); }
#line 1666 "parser.c"
    break;

  case 71: /* factor: atom  */
#line 235 "parser.y"
           { (yyval.expression) = (yyvsp[0].expression); }
#line 1672 "parser.c"
    break;

  case 72: /* function_call: NAME L_PAREN arguments R_PAREN  */
#line 239 "parser.y"
                                     { (yyval.expression) = create_expression(EXPR_FUNCTION_CALL, create_function_call((yyvsp[-3].str), (yyvsp[-1].arguments))); }
#line 1678 "parser.c"
    break;

  case 73: /* arguments: arguments COMMA expression  */
#line 243 "parser.y"
                                 { (yyval.arguments) = append_arguments((yyvsp[-2].arguments), (yyvsp[0].expression)); }
#line 1684 "parser.c"
    break;

  case 74: /* arguments: expression  */
#line 244 "parser.y"
                 { (yyval.arguments) = create_arguments((yyvsp[0].expression)); }
#line 1690 "parser.c"
    break;

  case 75: /* arguments: %empty  */
#line 245 "parser.y"
                  { (yyval.arguments) = NULL; }
#line 1696 "parser.c"
    break;

  case 76: /* atom: NAME  */
#line 249 "parser.y"
           { (yyval.expression) = create_expression(EXPR_NAME, create_name((yyvsp[0].str))); }
#line 1702 "parser.c"
    break;

  case 77: /* atom: DEC_INTEGER  */
#line 250 "parser.y"
                  { (yyval.expression) = create_expression(EXPR_INT, create_int((yyvsp[0].num))); }
#line 1708 "parser.c"
    break;

  case 78: /* atom: HEX_INTEGER  */
#line 251 "parser.y"
                  { (yyval.expression) = create_expression(EXPR_INT, create_int((yyvsp[0].num))); }
#line 1714 "parser.c"
    break;

  case 79: /* atom: L_PAREN expression R_PAREN  */
#line 252 "parser.y"
                                 { (yyval.expression) = (yyvsp[-1].expression); }
#line 1720 "parser.c"
    break;


#line 1724 "parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

