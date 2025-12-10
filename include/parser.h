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

#ifndef YY_YY_BUILD_PARSER_TAB_H_INCLUDED
# define YY_YY_BUILD_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 20 "src/parser/parser.y"

  #include "data_types.h"
  #include "../src/semantic/declarator_list.h"
  #include "../src/semantic/control/control.h"

#line 55 "build/parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    TK_INT = 258,                  /* TK_INT  */
    TK_FLOAT = 259,                /* TK_FLOAT  */
    TK_BOOL = 260,                 /* TK_BOOL  */
    TK_ID = 261,                   /* TK_ID  */
    TK_INT_LIT = 262,              /* TK_INT_LIT  */
    TK_FLOAT_LIT = 263,            /* TK_FLOAT_LIT  */
    TK_TRUE = 264,                 /* TK_TRUE  */
    TK_FALSE = 265,                /* TK_FALSE  */
    TK_ASSIGN = 266,               /* TK_ASSIGN  */
    TK_COMMA = 267,                /* TK_COMMA  */
    TK_NEWLINE = 268,              /* TK_NEWLINE  */
    TK_SEMICOLON = 269,            /* TK_SEMICOLON  */
    TK_DOT = 270,                  /* TK_DOT  */
    TK_EOF = 271,                  /* TK_EOF  */
    TK_PLUS = 272,                 /* TK_PLUS  */
    TK_MINUS = 273,                /* TK_MINUS  */
    TK_MULT = 274,                 /* TK_MULT  */
    TK_DIV = 275,                  /* TK_DIV  */
    TK_MOD = 276,                  /* TK_MOD  */
    TK_POW = 277,                  /* TK_POW  */
    TK_LPAREN = 278,               /* TK_LPAREN  */
    TK_RPAREN = 279,               /* TK_RPAREN  */
    TK_LBRACKET = 280,             /* TK_LBRACKET  */
    TK_RBRACKET = 281,             /* TK_RBRACKET  */
    TK_GT = 282,                   /* TK_GT  */
    TK_GE = 283,                   /* TK_GE  */
    TK_LT = 284,                   /* TK_LT  */
    TK_LE = 285,                   /* TK_LE  */
    TK_EQ = 286,                   /* TK_EQ  */
    TK_NE = 287,                   /* TK_NE  */
    TK_NOT = 288,                  /* TK_NOT  */
    TK_AND = 289,                  /* TK_AND  */
    TK_OR = 290,                   /* TK_OR  */
    TK_IF = 291,                   /* TK_IF  */
    TK_THEN = 292,                 /* TK_THEN  */
    TK_ELSE = 293,                 /* TK_ELSE  */
    TK_FI = 294,                   /* TK_FI  */
    TK_REPEAT = 295,               /* TK_REPEAT  */
    TK_DO = 296,                   /* TK_DO  */
    TK_DONE = 297                  /* TK_DONE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 26 "src/parser/parser.y"

    char *text;
    DATA_TYPE data_type;
    DeclaratorListNode *decl_list;
    LoopContext loop_ctx;
    int int_val;
    float float_val;
    int bool_val;
    ExprResult expr_val;

#line 125 "build/parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_BUILD_PARSER_TAB_H_INCLUDED  */
