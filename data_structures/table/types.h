//
// Created by User on 29/02/2024.
//

#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

typedef struct {
    char *label;
    void *value;
    node *next;
    node *previous;
} node;

typedef struct {
    node *root;
} table;


#endif //ASSEMBLER_TYPES_H
