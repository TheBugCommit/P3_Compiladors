#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../codegen/backpatch.h"
#include "../../codegen/codegen.h"
#include "../../codegen/gen_temps.h"
#include "../../utils/error.h"
#include "../../utils/log.h"
#include "../expression/expression_utils.h"
#include "../handlers/handlers.h"
#include "control.h"

LoopContext handle_repeat_start(ExprResult count_expr) {
    check_types_match(count_expr.type, TYPE_INT);
    control_push(SCOPE_LOOP);

    LoopContext ctx;
    ctx.counter_addr = gen_new_temp();
    emit_assign(ctx.counter_addr, "0");

    if (!is_temporary(count_expr.addr)) {
        ctx.bound_addr = gen_new_temp();
        emit_assign(ctx.bound_addr, count_expr.addr);
    } else {
        ctx.bound_addr = strdup(count_expr.addr);
    }

    ctx.start_label = gen_get_label();
    emit_if_goto(REL_LTI, ctx.counter_addr, ctx.bound_addr, -1);
    ctx.true_list = makelist(ctx.start_label);

    int goto_instr = gen_get_label();
    emit_goto(-1);
    ctx.false_list = makelist(goto_instr);

    if (count_expr.addr) free(count_expr.addr);
    log_production("Control: Repeat Start (Count 0 to %s)", ctx.bound_addr);

    return ctx;
}

void handle_repeat_end(LoopContext ctx, int start_label) {
    emit_binary(OC_ADDI, ctx.counter_addr, ctx.counter_addr, "1");
    emit_goto(ctx.start_label);

    backpatch(ctx.true_list, start_label);

    ListInt* breaks = control_pop();
    ctx.false_list = merge(ctx.false_list, breaks);

    int end_label = gen_get_label();
    backpatch(ctx.false_list, end_label);

    log_production("Control: Repeat End");

    gen_release_temp(ctx.counter_addr);
    gen_release_temp(ctx.bound_addr);
    if (ctx.counter_addr) free(ctx.counter_addr);
    if (ctx.bound_addr) free(ctx.bound_addr);
}

LoopContext handle_while_start(ExprResult loop_expression, int check_label) {
    check_types_match(loop_expression.type, TYPE_BOOL);
    control_push(SCOPE_LOOP);

    LoopContext ctx;
    ctx.start_label = check_label;

    convert_to_flow(&loop_expression);

    ctx.true_list = loop_expression.true_list;
    ctx.false_list = loop_expression.false_list;

    return ctx;
}

void handle_while_end(LoopContext ctx, int start_label) {
    emit_goto(ctx.start_label);

    backpatch(ctx.true_list, start_label);

    int end = gen_get_label();
    ctx.false_list = merge(ctx.false_list, control_pop());
    backpatch(ctx.false_list, end);
}

LoopContext handle_dountil_start(int start_label) {
    control_push(SCOPE_LOOP);
    LoopContext ctx;

    ctx.start_label = start_label;
    return ctx;
}

void handle_dountil_end(LoopContext ctx, ExprResult loop_expression) {
    check_types_match(loop_expression.type, TYPE_BOOL);
    convert_to_flow(&loop_expression);

    ctx.true_list = loop_expression.true_list;
    ctx.false_list = loop_expression.false_list;

    backpatch(ctx.true_list, ctx.start_label);

    int end = gen_get_label();
    ctx.false_list = merge(ctx.false_list, control_pop());
    backpatch(ctx.false_list, end);
}

LoopRange handle_loop_range(ExprResult start, ExprResult end) {
    check_types_match(start.type, TYPE_INT);
    check_types_match(end.type, TYPE_INT);

    LoopRange range;
    range.start = start;
    range.end = end;

    return range;
}

LoopContext handle_for_start(char* iterator_name, LoopRange range) {
    control_push(SCOPE_LOOP);

    DeclaratorListNode* var = create_declarator(strdup(iterator_name), 0, 0);
    handle_simple_declaration(TYPE_INT, var);
    handle_assigment_sentence(strdup(iterator_name), clone_result(range.start));

    LoopContext ctx;
    ctx.counter_addr = strdup(iterator_name);
    ctx.bound_addr = strdup(range.end.addr);

    int iterations = 0;
    if (range.start.is_constant && range.end.is_constant) {
        iterations = range.end.value.int_val - range.start.value.int_val + 1;
    }

    if (iterations > 0 && iterations <= UNROLL_LIMIT) {
        log_production("Optimization: Loop Unrolling activated (%d iterations)",
                       iterations);
        ctx.is_unrolled = 1;
        ctx.unroll_count = iterations;
        ctx.start_label = gen_get_label();
        ctx.true_list = NULL;
        ctx.false_list = NULL;
    } else {
        ctx.is_unrolled = 0;

        ctx.start_label = gen_get_label();
        emit_if_goto(REL_LEI, ctx.counter_addr, ctx.bound_addr, -1);
        ctx.true_list = makelist(ctx.start_label);

        int goto_instr = gen_get_label();
        emit_goto(-1);
        ctx.false_list = makelist(goto_instr);
    }

    if (range.start.addr) free(range.start.addr);
    if (range.end.addr) free(range.end.addr);

    log_production("Control: FOR Start (%s in range)", ctx.counter_addr);

    return ctx;
}

void handle_for_end(LoopContext ctx, int start_label) {
    ListInt* breaks = control_pop();

    if (ctx.is_unrolled) {
        int body_end_label = gen_get_label();
        for (int i = 1; i < ctx.unroll_count; i++) {
            emit_binary(OC_ADDI, ctx.counter_addr, ctx.counter_addr, "1");
            int guard_instr = gen_get_label();
            emit_if_goto(REL_GTI, ctx.counter_addr, ctx.bound_addr, -1);
            breaks = merge(breaks, makelist(guard_instr));

            ListInt* new_breaks =
                code_copy_and_relocate(ctx.start_label, body_end_label);
            breaks = merge(breaks, new_breaks);
        }
    } else {
        emit_binary(OC_ADDI, ctx.counter_addr, ctx.counter_addr, "1");
        emit_goto(ctx.start_label);
    }

    backpatch(ctx.true_list, start_label);

    int end_label = gen_get_label();
    ctx.false_list = merge(ctx.false_list, breaks);
    backpatch(ctx.false_list, end_label);

    log_production("Control: FOR End");

    if (ctx.counter_addr) free(ctx.counter_addr);
    gen_release_temp(ctx.bound_addr);
    if (ctx.bound_addr) free(ctx.bound_addr);
}