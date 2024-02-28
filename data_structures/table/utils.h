//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

typedef struct {
    char *label;
    void *value;
    node *next;
} node;

typedef struct {
    node *root;
} table;

void setValue(char *label, void *value);

void *getValue(char *label);

typedef void (*iterator_function)(char *, void *);

void iterate(table table, iterator_function callback);

#endif //ASSEMBLER_UTILS_H
