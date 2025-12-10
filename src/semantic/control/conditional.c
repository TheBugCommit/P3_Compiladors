#include "control.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../codegen/codegen.h"
#include "../../codegen/backpatch.h"
#include "../../utils/error.h"
#include "../../utils/log.h"
#include "../expression/expression_utils.h"

void handle_if_condition_check(ExprResult* expr) {
    if (expr->type != TYPE_BOOL) {
        REPORT_SEMANTIC_ERROR(current_location(), 
            "IF condition must be BOOLEAN (got %s)", TYPE_NAME[expr->type]);

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