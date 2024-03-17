//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_LIST_UTILS_H
#define ASSEMBLER_LIST_UTILS_H

#include "stdbool.h"
#include "./types.h"

void addFirst(list *list, void *value);

void addLast(list *list, void *value);

void *setNth(list *list, unsigned int index, void *value);

node *getFirst(list *list);

node *getLast(list *list);

node *getNth(list *list, unsigned int index);

typedef bool (*search_function)(void *, void *);

node *search(list *list, search_function callback, void *comparedData);

typedef void (*delete_function)(void *);

void deleteNth(list *list, unsigned int index, delete_function callback);

void deleteNode(node *node, delete_function callback);

void dispose(list **list, delete_function callback);

typedef void (*iterator_function)(unsigned int index, void *);

void iterate(list *list, iterator_function callback);

#endif //ASSEMBLER_LIST_UTILS_H
