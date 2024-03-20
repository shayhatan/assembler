//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_TABLE_UTILS_H
#define ASSEMBLER_TABLE_UTILS_H

#include "types.h"

void setValue(table *table, char *label, void *value);

void *getValue(table *table, char *label);


typedef void (*DeleteFn)(void *);

void deleteKey(table *table, char *label, DeleteFn callback);

void dispose_table(table **table, DeleteFn callback);

void iterate_table(table *table, iterator_function callback);

void init_table(table *table);

#endif //ASSEMBLER_TABLE_UTILS_H
