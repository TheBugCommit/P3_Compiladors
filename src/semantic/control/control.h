#ifndef CONTROL_H
#define CONTROL_H

#include "../expression/expression.h"

typedef struct {
    char* counter_addr;
    char* bound_addr;  
} LoopContext;

LoopContext handle_repeat_start(ExprResult count_expr);
void handle_repeat_end(LoopContext ctx, int start_label);

void handle_if_condition_check(ExprResult* expr);
void handle_if_then_block(ExprResult* expr, int start_label);
void handle_if_end(ExprResult* expr);
void handle_if_else_transition(ExprResult* expr, int start_else_label);
void handle_if_else_end(int jump_from_then_label);

#endif