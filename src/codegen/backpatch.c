#include "backpatch.h"
#include <stdlib.h>
#include <stdio.h>
#include "codegen.h"

ListInt* makelist(int label) {
    ListInt* node = malloc(sizeof(ListInt));
    node->label = label;
    node->next = NULL;
    return node;
}

ListInt* merge(ListInt* l1, ListInt* l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    ListInt* current = l1;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = l2;
    return l1;
}

void backpatch(ListInt* l, int target_label) {
    ListInt* current = l;
    while (current != NULL) {
        patch_quad(current->label, target_label);
        
        ListInt* temp = current;
        current = current->next;
        free(temp);
    }
}