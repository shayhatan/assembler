//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_LIST_TYPES_H
#define ASSEMBLER_LIST_TYPES_H


typedef struct {
    char *label;
    void *value;
    node *next;
    node *previous;
} node;

typedef struct {
    node *root;
} list;

#endif //ASSEMBLER_LIST_TYPES_H
