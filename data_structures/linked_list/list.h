//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_LIST_UTILS_H
#define ASSEMBLER_LIST_UTILS_H

#include "stdbool.h"

typedef struct node {
    void *value;
    struct node *next;
    struct node *previous;
} Node;

typedef void (*DeleteFn)(void *);

typedef struct {
    Node *root;

    DeleteFn freeElement;
} *List;

void addFirst(List list, void *value);

void addLast(List list, void *value);

void *setNth(List list, unsigned int index, void *value);

Node *getFirst(List list);

Node *getLast(List list);

Node *getNth(List list, unsigned int index);

typedef bool (*search_function)(void *, void *);

Node *search(List list, search_function callback, void *comparedData);

void deleteNth(List list, unsigned int index);

void deleteNode(List list, Node *node);

void listDispose(List list);

typedef void (*iterator_function)(unsigned int index, void *);

void iterate(List list, iterator_function callback);

int listLength(List list);

List init_list(DeleteFn deleteElementCallback);

#endif //ASSEMBLER_LIST_UTILS_H
