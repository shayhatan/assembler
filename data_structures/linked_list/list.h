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


typedef enum ListResult {
    LIST_SUCCESS,
    LIST_OUT_OF_MEMORY,
    LIST_ERROR,
    LIST_NOT_INITIALIZED,
    LIST_NULL_NODE
} ListResult;

enum ListResult listEnqueue(List list, void *value);

enum ListResult listPush(List list, void *value);

enum ListResult listSetNth(List list, unsigned int index, void *value);

Node *listGetFirst(List list);

Node *listGetLast(List list);

Node *listGetNth(List list, unsigned int index);

typedef bool (*search_function)(void *, void *);

Node *listSearch(List list, search_function callback, void *comparedData);

enum ListResult listDeleteNth(List list, unsigned int index);

enum ListResult listDeleteNode(List list, Node *node);

enum ListResult listDispose(List list);

typedef int (*iterator_function)(unsigned int index, void *);

int listIterate(List list, iterator_function callback);

int listLength(List list);

List listCreate(DeleteFn freeElement);

#endif //ASSEMBLER_LIST_UTILS_H
