//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_LIST_UTILS_H
#define ASSEMBLER_LIST_UTILS_H

#include "./types.h"

void addFirst(list *list, void *value);

void addLast(list *list, void *value);

void setNth(list *list, unsigned int index, void *value);

void *getFirst(list *list, unsigned int index);

void *getLast(list *list, unsigned int index);

void *getNthValue(list *list, unsigned int index);

typedef void (*delete_function)(void *);

void *deleteNth(list *list, unsigned int index, delete_function callback);

void dispose(list **list);

typedef void (*iterator_function)(unsigned int index, void *);

void iterate(list list, iterator_function callback);

#endif //ASSEMBLER_LIST_UTILS_H
