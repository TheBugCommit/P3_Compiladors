#include <stdlib.h>
#include <string.h>

#include "../../codegen/backpatch.h"
#include "../../codegen/codegen.h"
#include "../../codegen/gen_temps.h"
#include "../../utils/error.h"
#include "expression.h"
#include "expression_priv.h"
#include "expression_utils.h"

static int fold_relational(ExprResult a, ExprResult b, char* op) {
    double val_a = get_value(a);
    double val_b = get_value(b);

    if (strncmp(op, REL_EQ, 2) == 0) return val_a == val_b;
    if (strncmp(op, REL_NE, 2) == 0) return val_a != val_b;
    if (strncmp(op, REL_GT, 2) == 0) return val_a > val_b;
    if (strncmp(op, REL_LT, 2) == 0) return val_a < val_b;
    if (strncmp(op, REL_GE, 2) == 0) return val_a >= val_b;
    if (strncmp(op, REL_LE, 2) == 0) return val_a <= val_b;
    return 0;
}

ExprResult eval_relational(ExprResult a, ExprResult b, char* op) {
    ExprResult res = {TYPE_BOOL};
    if (!is_numeric(&a) || !is_numeric(&b)) return res;

    /*if (a.is_constant && b.is_constant) {
        int result = fold_relational(a, b, op);
        cleanup_operands(a, b);
        return make_literal_result(TYPE_BOOL, result);
    }*/

    char final_op[10];
    if (a.type == TYPE_FLOAT || b.type == TYPE_FLOAT) {
        a = ensure_type(a, TYPE_FLOAT);
        b = ensure_type(b, TYPE_FLOAT);
        sprintf(final_op, "%sF", op);
    } else {
        sprintf(final_op, "%sI", op);
    }

    int true_label = gen_get_label();
    emit_if_goto(final_op, a.addr, b.addr, -1);
    res.true_list = makelist(true_label);

    int false_label = gen_get_label();
    emit_goto(-1);
    res.false_list = makelist(false_label);

    cleanup_operands(a, b);
    return res;
}

ExprResult eval_or(ExprResult a, int m_label, ExprResult b) {
    /*if (a.is_constant) {
        if (a.value.bool_val) {
            int next_instr = gen_get_label();
            backpatch(b.true_list, next_instr);
            backpatch(b.false_list, next_instr);
            
            if (b.addr) {
                gen_release_temp(b.addr);
                free(b.addr);
            }

            return make_literal_result(TYPE_BOOL, 1);
        } else {
            if (a.addr) free(a.addr);
            return b;
        }
    }

    if (b.is_constant) {
        if (b.value.bool_val) {
            ExprResult res = {TYPE_BOOL};
            res.true_list = merge(a.true_list, a.false_list);
            res.false_list = NULL;
            return res;
        } else { // Si B és fals
            return a;
        }
    }*/

    convert_to_flow(&a);
    convert_to_flow(&b);

    backpatch(a.false_list, m_label);

    ExprResult res = {TYPE_BOOL};
    res.true_list = merge(a.true_list, b.true_list);
    res.false_list = b.false_list;
    return res;
}

ExprResult eval_and(ExprResult a, int m_label, ExprResult b) {
    /*if (a.is_constant) {
        if (!a.value.bool_val) {
            int next_instr = gen_get_label();
            backpatch(b.true_list, next_instr);
            backpatch(b.false_list, next_instr);
            
            if (b.addr) {
                gen_release_temp(b.addr);
                free(b.addr);
            }
            return make_literal_result(TYPE_BOOL, 0);
        } else {
            if (a.addr) free(a.addr);
            return b;
        }
    }*/

    convert_to_flow(&a);
    convert_to_flow(&b);

    backpatch(a.true_list, m_label);

    ExprResult res = {TYPE_BOOL};
    res.false_list = merge(a.false_list, b.false_list);
    res.true_list = b.true_list;
    return res;
}

ExprResult eval_not(ExprResult a) {
    /*if (a.is_constant) {
        int val = (a.value.bool_val == 0) ? 1 : 0;
        if (a.addr) free(a.addr);
        return make_literal_result(TYPE_BOOL, val);
    }*/

    convert_to_flow(&a);

    ExprResult res = a;
    ListInt* temp = res.true_list;
    res.true_list = res.false_list;
    res.false_list = temp;
    return res;
}