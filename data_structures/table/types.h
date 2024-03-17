//
// Created by User on 29/02/2024.
//

#ifndef ASSEMBLER_TABLE_TYPES_H
#define ASSEMBLER_TABLE_TYPES_H

#include "../linked_list/types.h"

typedef struct {
    char *label;
    void *value;
} key_value;

typedef struct {
    list *list;
} table;


#endif //ASSEMBLER_TABLE_TYPES_H
