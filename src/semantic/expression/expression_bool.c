#include <stdlib.h>
#include <string.h>

#include "../../codegen/backpatch.h"
#include "../../codegen/codegen.h"
#include "../../codegen/gen_temps.h"
#include "../../utils/error.h"
#include "expression.h"
#include "expression_priv.h"
#include "expression_utils.h"

ExprResult eval_relational(ExprResult a, ExprResult b, char* op) {
    ExprResult res = {TYPE_BOOL};
    if (!is_numeric(&a) || !is_numeric(&b)) return res;

    char final_op[10];
    char type;
    if (a.type == TYPE_FLOAT || b.type == TYPE_FLOAT) {
        a = ensure_type(a, TYPE_FLOAT);
        b = ensure_type(b, TYPE_FLOAT);
        type = 'F';
    } else {
        type = 'I';
    }
    if(!(strcmp(op, REL_EQ) == 0 || strcmp(op, REL_NE) == 0)) {
        sprintf(final_op, "%s%c", op, type);
    } else {
        sprintf(final_op, "%s", op);
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
    convert_to_flow(&a);
    convert_to_flow(&b);

    backpatch(a.false_list, m_label);

    ExprResult res = {TYPE_BOOL};
    res.true_list = merge(a.true_list, b.true_list);
    res.false_list = b.false_list;
    return res;
}

ExprResult eval_and(ExprResult a, int m_label, ExprResult b) {
    convert_to_flow(&a);
    convert_to_flow(&b);

    backpatch(a.true_list, m_label);

    ExprResult res = {TYPE_BOOL};
    res.false_list = merge(a.false_list, b.false_list);
    res.true_list = b.true_list;
    return res;
}

ExprResult eval_not(ExprResult a) {
    convert_to_flow(&a);

    ExprResult res = a;
    ListInt* temp = res.true_list;
    res.true_list = res.false_list;
    res.false_list = temp;
    return res;
}