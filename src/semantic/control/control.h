#ifndef CONTROL_H
#define CONTROL_H

#include "../expression/expression.h"

typedef struct {
    char* counter_addr;
    char* bound_addr;
    
    int check_label;    
    ListInt* true_list; 
    ListInt* false_list;
} LoopContext;

typedef struct {
    char* literal_addr;
    int label;
} CaseEntry;

LoopContext handle_repeat_start(ExprResult count_expr);
void handle_repeat_end(LoopContext ctx, int start_label);

void handle_if_condition_check(ExprResult* expr);
void handle_if_then_block(ExprResult* expr, int start_label);
void handle_if_end(ExprResult* expr);
void handle_if_else_transition(ExprResult* expr, int start_else_label);
void handle_if_else_end(int jump_from_then_label);

void handle_switch_start(ExprResult expr);
void handle_case_start(ExprResult case_literal);
void handle_default_start();
void handle_switch_break();
void handle_switch_end();

#endif