#ifndef CONTROL_H
#define CONTROL_H

#include "data_types.h"
#include "../handlers/handlers_utils.h"

typedef struct {
    char* counter_addr;
    char* bound_addr;
    
    int start_label;    
    ListInt* true_list; 
    ListInt* false_list;
} LoopContext;

typedef enum { SCOPE_LOOP, SCOPE_SWITCH } ScopeType;

void control_push(ScopeType type);
void control_add_break(int label);
ListInt* control_pop();
void handle_generic_break();

LoopContext handle_repeat_start(ExprResult count_expr);
void handle_repeat_end(LoopContext ctx, int start_label);
LoopContext handle_while_start(ExprResult loop_expression, int check_label);
void handle_while_end(LoopContext ctx, int start_label);
LoopContext handle_dountil_start(int start_label);
void handle_dountil_end(LoopContext ctx, ExprResult loop_expression);

void handle_if_condition_check(ExprResult* expr);
void handle_if_then_block(ExprResult* expr, int start_label);
void handle_if_end(ExprResult* expr);
void handle_if_else_transition(ExprResult* expr, int start_else_label);
void handle_if_else_end(int jump_from_then_label);

void handle_switch_start(ExprResult expr);
void handle_case_start(ExprResult case_literal);
void handle_default_start();
void handle_switch_end();

#endif