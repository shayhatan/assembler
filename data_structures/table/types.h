//
// Created by User on 29/02/2024.
//

#ifndef ASSEMBLER_TABLE_TYPES_H
#define ASSEMBLER_TABLE_TYPES_H

#include "../linked_list/list.h"

typedef struct key_value {
    char *label;
    void *value;
} key_value;

typedef struct {
    List list;
} table;


#endif //ASSEMBLER_TABLE_TYPES_H
