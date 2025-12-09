#ifndef BACKPATCH_H
#define BACKPATCH_H

#include "data_types.h"

ListInt* makelist(int label);
ListInt* merge(ListInt* l1, ListInt* l2);
void backpatch(ListInt* l, int target_label);

#endif