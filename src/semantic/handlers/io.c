#include <stdlib.h>

#include "../../codegen/codegen.h"
#include "../../codegen/gen_temps.h"
#include "../../utils/error.h"
#include "../../utils/log.h"
#include "../expression/expression_utils.h"
#include "handlers.h"

void handle_put_statement(ExprResult expr) {
    if (expr.is_array) {
        REPORT_SEMANTIC_ERROR(current_location(), "Cannot put array.");
        if (expr.addr) free(expr.addr);
        return;
    }

    if (expr.type == TYPE_BOOL) {
        ensure_boolean_value(&expr);
    }

    log_production("IO: Generating PUT for %s expression",
                   TYPE_NAME[expr.type]);

    const char* proc_name = (expr.type == TYPE_FLOAT) ? PROC_PUTF : PROC_PUTI;

    emit_io(expr.addr, proc_name, 1);

    gen_release_temp(expr.addr);
    if (expr.addr) free(expr.addr);
}