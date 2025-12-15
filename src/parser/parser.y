%{
#include <stdio.h>
#include <string.h>
#include "data_types.h"
#include "globals.h"
#include "../src/utils/log.h"
#include "../src/symtab/symtab.h"
#include "../src/utils/error.h"
#include "../src/semantic/declarator_list.h"
#include "../src/semantic/handlers/handlers.h"
#include "../src/semantic/expression/expression.h"
#include "../src/semantic/expression/expression_utils.h"
#include "../src/semantic/control/control.h"
#include "../src/codegen/codegen.h"

extern int yylex();

%}

%code requires {
  #include "data_types.h"
  #include "../src/semantic/declarator_list.h"
  #include "../src/semantic/control/control.h"
}

%union {
    char *text;
    DATA_TYPE data_type;
    DeclaratorListNode *decl_list;
    LoopContext loop_ctx;
    LoopRange loop_range;
    int int_val;
    float float_val;
    int bool_val;
    ExprResult expr_val;
}
%locations
/* Type Tokens */
%token <data_type> TK_INT TK_FLOAT TK_BOOL

/* IDs Tokens */
%token <text> TK_ID

%token <int_val> TK_INT_LIT 
%token <float_val> TK_FLOAT_LIT
%token <bool_val> TK_TRUE TK_FALSE

/* Puntuation and Operation Tokens  */
%token TK_ASSIGN TK_COMMA TK_NEWLINE TK_SEMICOLON TK_COLON TK_DOT TK_RANGE TK_EOF
%token TK_PLUS TK_MINUS TK_MULT TK_DIV TK_MOD TK_POW
%token TK_LPAREN TK_RPAREN TK_LBRACKET TK_RBRACKET
%token TK_GT TK_GE TK_LT TK_LE TK_EQ TK_NE
%token TK_NOT TK_AND TK_OR

/* Stroctures*/
%token TK_IF TK_THEN TK_ELSE TK_FI
%token TK_REPEAT TK_WHILE TK_FOR TK_DO TK_IN TK_DONE TK_UNTIL
%token TK_SWITCH TK_CASE TK_BREAK TK_DEFAULT TK_FSWITCH 

%type <data_type> primitive_type
%type <expr_val>  arith_expr arith_term unary_expr arith_factor primary literal 
%type <expr_val>  expression bool_term bool_factor relational_expr
%type <expr_val>  if_condition
%type <decl_list> declarator_list declarator
%type <int_val> M N
%type <loop_ctx> repeat_start_action while_start_action do_until_start_action for_start_action
%type <loop_range> loop_range
%define parse.error detailed

%%

program:
    optional_newlines item_list optional_newlines opt_eof
;

item_list:
    program_item
    | item_list optional_newlines program_item
    | item_list error end_of_stmt
      {
        log_production("Recovered from item list error, skipping item.");
        yyerrok;
      }
;

program_item:
    simple_declaration
    | sentence
;

sentence_list:
    | sentence_list sentence
    | sentence_list TK_NEWLINE
;

optional_newlines:
    /* void */
    | optional_newlines TK_NEWLINE
;

end_of_stmt:
    TK_NEWLINE
    | TK_EOF 
;

opt_eof:
      /* void */
    | TK_EOF
;


simple_declaration: 
    primitive_type declarator_list end_of_stmt { handle_simple_declaration($1, $2); }
    | primitive_type declarator_list error
    {
        log_production("Recovered from missing \\n");
        declarator_list_free($2);
        yyerrok;
    }
    | primitive_type error end_of_stmt
    {
        log_production("Recovered from identifier list error");
        yyerrok;
    }
;

primitive_type:
    TK_INT      { $$ = TYPE_INT; log_production("Type: int"); }
    | TK_FLOAT  { $$ = TYPE_FLOAT; log_production("Type: float"); }
    | TK_BOOL  { $$ = TYPE_BOOL; log_production("Type: bool"); }
;

declarator_list:
    declarator { $$ = $1; }
    | declarator_list TK_COMMA declarator { $$ = declarator_list_prepend($1, $3); }
;

declarator:
    TK_ID { $$ = create_declarator($1, 0, 0); }
    | TK_ID TK_LBRACKET TK_INT_LIT TK_RBRACKET { $$ = create_declarator($1, 1, $3); }
;

sentence:
    assignment_sentence end_of_stmt
    | expression_sentence end_of_stmt
    | iterative_sentence end_of_stmt
    | conditional_sentence end_of_stmt
    | TK_BREAK TK_SEMICOLON { handle_generic_break(); }
;

M:  { $$ = gen_get_label(); log_production("Marker M: Label %d", $$); };

N: { 
    $$ = gen_get_label();
    emit_goto(-1);
    log_production("Marker N: Generated GOTO at %d", $$);
};

conditional_sentence:
    if_sentence
    | switch_sentence
;

if_condition:
    TK_IF expression TK_THEN { handle_if_condition_check(&$2); }
    M { handle_if_then_block(&$2, $5); $$ = $2; }
;

if_sentence:
    if_condition sentence_list TK_FI { handle_if_end(&$1); }
    | if_condition sentence_list N TK_ELSE M { handle_if_else_transition(&$1, $5); }
    sentence_list TK_FI { handle_if_else_end($3); }
;

switch_sentence:
    TK_SWITCH TK_LPAREN expression TK_RPAREN optional_newlines { handle_switch_start($3); }
    case_list TK_FSWITCH { handle_switch_end(); }
;
case_list:
    case_item
    | case_list case_item
;
case_item:
    case_entry
    | case_default
;
case_entry:
    TK_CASE literal TK_COLON { handle_case_start($2); }
    sentence_list
;
case_default:
    TK_DEFAULT TK_COLON { handle_default_start(); }
    sentence_list
;
iterative_sentence:
    repeat_sentence
    | while_sentence
    | do_until_sentence
    | for_sentence
;
repeat_start_action:
    TK_REPEAT arith_expr optional_newlines TK_DO { $$ = handle_repeat_start($2); }
;

repeat_sentence:
    repeat_start_action M sentence_list TK_DONE { handle_repeat_end($1, $2); }
;

while_start_action:
    TK_WHILE M expression optional_newlines TK_DO { $$ = handle_while_start($3, $2); }
;
while_sentence:
    while_start_action M sentence_list TK_DONE { handle_while_end($1, $2); }
;

do_until_start_action:
    TK_DO M { $$ = handle_dountil_start($2); }
;
do_until_sentence:
    do_until_start_action sentence_list TK_UNTIL expression { handle_dountil_end($1, $4); }
;

loop_range:
    arith_expr TK_RANGE arith_expr { $$ = handle_loop_range($1, $3); }

for_start_action:
    TK_FOR TK_ID TK_IN loop_range optional_newlines TK_DO { $$ = handle_for_start($2, $4); }
;

for_sentence: 
    for_start_action M sentence_list TK_DONE { handle_for_end($1, $2); }
;

assignment_sentence:
    TK_ID TK_ASSIGN expression { handle_assigment_sentence($1, $3); }
    | TK_ID TK_LBRACKET arith_expr TK_RBRACKET TK_ASSIGN expression { handle_array_assignment($1, $3, $6); }
;

expression_sentence:
    expression { handle_put_statement($1); }
;

/* Level 1: OR */
expression:
    expression TK_OR M bool_term { $$ = eval_or($1, $3, $4); }
    | bool_term { $$ = $1; }
;

/* Level 2: AND */
bool_term:
    bool_term TK_AND M bool_factor { $$ = eval_and($1, $3, $4); }
    | bool_factor { $$ = $1; }
;

/* Level 3: NOT */
bool_factor:
    TK_NOT bool_factor { $$ = eval_not($2); }
    | relational_expr { $$ = $1; }
;

/* Level 4: Relationals */
relational_expr:
    arith_expr TK_GT arith_expr { $$ = eval_relational($1, $3, REL_GT); }
    | arith_expr TK_LT arith_expr { $$ = eval_relational($1, $3, REL_LT); }
    | arith_expr TK_GE arith_expr { $$ = eval_relational($1, $3, REL_GE); }
    | arith_expr TK_LE arith_expr { $$ = eval_relational($1, $3, REL_LE); }
    | arith_expr TK_EQ arith_expr { $$ = eval_relational($1, $3, REL_EQ); }
    | arith_expr TK_NE arith_expr { $$ = eval_relational($1, $3, REL_NE); }
    | arith_expr { $$ = $1; }
;

/* Level 5: Sum, Sub, Unary +/- */
arith_expr:
    arith_expr TK_PLUS unary_expr    { $$ = eval_add($1, $3); }
  | arith_expr TK_MINUS unary_expr   { $$ = eval_sub($1, $3); }
  | unary_expr                       { $$ = $1; }
;

/* Level 5.5: Unary */
unary_expr:
    TK_PLUS unary_expr  { $$ = eval_unary_plus($2); }
  | TK_MINUS unary_expr { $$ = eval_unary_minus($2); }
  | arith_term          { $$ = $1; }
;

/* Level 6: *, /, % */
arith_term:
    arith_term TK_MULT arith_factor { $$ = eval_mul($1, $3); }
  | arith_term TK_DIV arith_factor  { $$ = eval_div($1, $3); }
  | arith_term TK_MOD arith_factor  { $$ = eval_mod($1, $3); }
  | arith_factor                    { $$ = $1; }
;

/* Level 7: Pow (**) */
arith_factor:
    primary TK_POW arith_factor { $$ = eval_pow($1, $3); }
    | primary { $$ = $1; }
;

primary:
    TK_LPAREN expression TK_RPAREN { $$ = $2; log_production("Expression: Parenthesized"); }
    | literal { $$ = $1; }
    | TK_ID { $$ = handle_primary_id($1); }
    | primitive_type TK_LPAREN expression TK_RPAREN { $$ = eval_cast($1, $3); }
    | TK_ID TK_LBRACKET arith_expr TK_RBRACKET { $$ = handle_array_access($1, $3);}
;

literal:
    TK_INT_LIT     { $$ = make_literal_result(TYPE_INT, (float)$1); }
    | TK_FLOAT_LIT { $$ = make_literal_result(TYPE_FLOAT, $1); }
    | TK_TRUE      { $$ = make_literal_result(TYPE_BOOL, $1); }
    | TK_FALSE     { $$ = make_literal_result(TYPE_BOOL, $1); }
;
%%