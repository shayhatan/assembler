//
// Created by User on 29/02/2024.
//

#ifndef ASSEMBLER_TABLE_TYPES_H
#define ASSEMBLER_TABLE_TYPES_H

typedef struct {
    char *label;
    void *value;
    node *next;
    node *previous;
} node;

typedef struct {
    node *root;
} table;


#endif //ASSEMBLER_TABLE_TYPES_H
