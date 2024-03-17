//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_TABLE_UTILS_H
#define ASSEMBLER_TABLE_UTILS_H

#include "types.h"

void setValue(table *table, char *label, void *value);

void *getValue(table *table, char *label);


typedef void (*delete_function)(void *);

void *deleteKey(table *table, char *label, delete_function callback);

void dispose(table **table, delete_function callback);

typedef void (*iterator_function)(char *, void *);

void iterate(table *table, iterator_function callback);

#endif //ASSEMBLER_TABLE_UTILS_H
