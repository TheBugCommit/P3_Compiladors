#include "control.h"
#include <stdlib.h>
#include <stdio.h>
#include "../../utils/error.h"
#include "../../codegen/codegen.h"
#include "../../codegen/backpatch.h"

typedef struct {
    ScopeType type;
    ListInt* break_list;
} ControlScope;

static ControlScope* control_stack = NULL;
static int control_top = -1;
static int control_stack_cap = 0;

static void ensure_free_stack_capacity(int min_size) {
    if (control_stack_cap >= min_size) return;

    int new_cap = (control_stack_cap == 0) ? 4 : control_stack_cap * 2;
    if (new_cap < min_size) new_cap = min_size;

    ControlScope* temp = realloc(control_stack, new_cap * sizeof(ControlScope));
    if (!temp) {
        fprintf(
            stderr,
            "FATAL ERROR: Out of memory reallocating switch stack.\n");
        exit(1);
    }

    control_stack = temp;
    control_stack_cap = new_cap;
}

void control_push(ScopeType type) {
    ensure_free_stack_capacity(control_stack_cap + 1);

    control_top++;
    control_stack[control_top].type = type;
    control_stack[control_top].break_list = NULL;
}

void control_add_break(int label) {
    if (control_top < 0) {
        REPORT_SEMANTIC_ERROR(current_location(), "BREAK outside of loop or switch");
        return;
    }
    
    control_stack[control_top].break_list = 
        merge(control_stack[control_top].break_list, makelist(label));
}

ListInt* control_pop() {
    if (control_top < 0) return NULL;
    
    ListInt* breaks = control_stack[control_top].break_list;
    control_top--;
    return breaks;
}

void handle_generic_break() {
    int label = gen_get_label();
    emit_goto(-1);
    control_add_break(label);
}