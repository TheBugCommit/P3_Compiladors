#include "control.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../codegen/codegen.h"
#include "../../codegen/backpatch.h"
#include "../../codegen/gen_temps.h"
#include "../../utils/error.h"
#include "../../utils/log.h"
#include "../expression/expression_utils.h"

LoopContext handle_repeat_start(ExprResult count_expr) {
    if (count_expr.type != TYPE_INT) {
        REPORT_SEMANTIC_ERROR(current_location(),
                              "Repeat expression must be an INTEGER (got %s)",
                              TYPE_NAME[count_expr.type]);
    }

    LoopContext ctx;
    ctx.counter_addr = gen_new_temp();
    emit_assign(ctx.counter_addr, "0");

    if(!is_temporary(count_expr.addr)) {
        ctx.bound_addr = gen_new_temp();
        emit_assign(ctx.bound_addr, count_expr.addr);
    } else {
        ctx.bound_addr = strdup(count_expr.addr);
    }

    ctx.check_label = gen_get_label();
    emit_if_goto(REL_LTI, ctx.counter_addr, ctx.bound_addr, -1);
    ctx.true_list = makelist(ctx.check_label);

    int goto_instr = gen_get_label();
    emit_goto(-1);
    ctx.false_list = makelist(goto_instr);

    if (count_expr.addr) free(count_expr.addr);
    log_production("Control: Repeat Start (Count 0 to %s)", ctx.bound_addr);
    
    return ctx;
}

void handle_repeat_end(LoopContext ctx, int start_label) {
    emit_binary(OC_ADDI, ctx.counter_addr, ctx.counter_addr, "1");
    emit_goto(ctx.check_label);

    backpatch(ctx.true_list, start_label);

    int next_inst = gen_get_label();
    backpatch(ctx.false_list, next_inst);

    log_production("Control: Repeat End");

    gen_release_temp(ctx.counter_addr);
    gen_release_temp(ctx.bound_addr);
    if (ctx.counter_addr) free(ctx.counter_addr);
    if (ctx.bound_addr) free(ctx.bound_addr);
}