#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../codegen/backpatch.h"
#include "../../codegen/codegen.h"
#include "../../utils/error.h"
#include "../../utils/log.h"
#include "../expression/expression_utils.h"
#include "control.h"

void handle_if_condition_check(ExprResult* expr) {
    if (expr->type != TYPE_BOOL) {
        REPORT_SEMANTIC_ERROR(current_location(),
                              "IF condition must be BOOLEAN (got %s)",
                              TYPE_NAME[expr->type]);

        return;
    }

    convert_to_flow(expr);
}
void handle_if_then_block(ExprResult* expr, int start_label) {
    backpatch(expr->true_list, start_label);
}

void handle_if_end(ExprResult* expr) {
    int end_label = gen_get_label();
    backpatch(expr->false_list, end_label);

    log_production("Control: IF statement closed");
}

void handle_if_else_transition(ExprResult* expr, int start_else_label) {
    backpatch(expr->false_list, start_else_label);
}

void handle_if_else_end(int jump_from_then_label) {
    int end_label = gen_get_label();

    ListInt* jump_list = makelist(jump_from_then_label);
    backpatch(jump_list, end_label);
    log_production("Control: IF-ELSE statement closed");
}

#define MAX_CASES 128

typedef struct {
    char* expr_addr;
    DATA_TYPE type;

    ListInt* jump_to_test;
    ListInt* break_list;

    CaseEntry* cases;
    int case_count;
    int cases_cap;

    int default_label;
} SwitchContext;

static SwitchContext* switch_stack = NULL;
static int switch_top = -1;
static int switch_stack_cap = 0;

static SwitchContext* current_switch() {
    if (switch_top < 0) return NULL;
    return &switch_stack[switch_top];
}

static void ensure_free_stack_capacity(int min_size) {
    if (switch_stack_cap >= min_size) return;

    int new_cap = (switch_stack_cap == 0) ? 2 : switch_stack_cap * 2;
    if (new_cap < min_size) new_cap = min_size;

    SwitchContext* temp = realloc(switch_stack, new_cap * sizeof(SwitchContext));
    if (!temp) {
        fprintf(
            stderr,
            "FATAL ERROR: Out of memory reallocating switch stack.\n");
        exit(1);
    }

    switch_stack = temp;
    switch_stack_cap = new_cap;
}

static void ensure_cases_capacity(CaseEntry** entries, int* case_cap, int min_size) {
    if (*case_cap >= min_size) return;

    int new_cap = (*case_cap == 0) ? 4 : *case_cap * 2;
    if (*case_cap < min_size) new_cap = min_size;

    CaseEntry* temp = realloc(*entries, new_cap * sizeof(CaseEntry));
    if (!temp) {
        fprintf(
            stderr,
            "FATAL ERROR: Out of memory reallocating cases entries capacity.\n");
        exit(1);
    }

    *case_cap = new_cap;
    *entries = temp;
}

void handle_switch_start(ExprResult expr) {
    ensure_free_stack_capacity(switch_stack_cap + 1);

    if (expr.type == TYPE_BOOL) ensure_boolean_value(&expr);

    switch_top++;
    SwitchContext* ctx = &switch_stack[switch_top];

    ctx->expr_addr = strdup(expr.addr);
    ctx->type = expr.type;
    ctx->break_list = NULL;
    ctx->cases = NULL;
    ctx->case_count = 0;
    ctx->cases_cap = 0;
    ctx->default_label = -1;

    int goto_instr = gen_get_label();
    emit_goto(-1);
    ctx->jump_to_test = makelist(goto_instr);

    if (expr.addr) free(expr.addr);

    log_production("Control: SWITCH Start");
}

void handle_case_start(ExprResult case_literal) {
    SwitchContext* ctx = current_switch();
    if (!ctx) return;

    ensure_cases_capacity(&ctx->cases, &ctx->cases_cap, ctx->case_count + 1);

    if (case_literal.type != ctx->type) {
        REPORT_SEMANTIC_ERROR(current_location(), "Case type mismatch");
    }

    int current_label = gen_get_label();

    ctx->cases[ctx->case_count].literal_addr = strdup(case_literal.addr);
    ctx->cases[ctx->case_count].label = current_label;
    ctx->case_count++;

    if (case_literal.addr) free(case_literal.addr);

    log_production("Control: CASE defined at label %d", current_label);
}

void handle_default_start() {
    SwitchContext* ctx = current_switch();
    if (!ctx) return;

    ctx->default_label = gen_get_label();
    log_production("Control: DEFAULT defined at label %d", ctx->default_label);
}

void handle_switch_break() {
    SwitchContext* ctx = current_switch();
    if (!ctx) {
        REPORT_SEMANTIC_ERROR(current_location(), "BREAK outside of switch");
        return;
    }

    int label = gen_get_label();
    emit_goto(-1);
    ctx->break_list = merge(ctx->break_list, makelist(label));
}

void handle_switch_end() {
    SwitchContext* ctx = current_switch();
    if (!ctx) return;

    int test_block_start = gen_get_label();
    backpatch(ctx->jump_to_test, test_block_start);

    log_production("Control: Generating SWITCH Test Block at %d",
                   test_block_start);

    for (int i = 0; i < ctx->case_count; i++) {
        char* op = (ctx->type == TYPE_FLOAT) ? REL_EQF : REL_EQI;
        emit_if_goto(op, ctx->expr_addr, ctx->cases[i].literal_addr,
                     ctx->cases[i].label);
        if (ctx->cases[i].literal_addr) free(ctx->cases[i].literal_addr);
    }

    emit_goto(ctx->default_label);

    int end_label = gen_get_label();
    backpatch(ctx->break_list, end_label);

    if (ctx->expr_addr) free(ctx->expr_addr);
    switch_top--;

    log_production("Control: SWITCH Closed");
}