#ifndef MMN14_SORTED_LIST_H
#define MMN14_SORTED_LIST_H

#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    void *data;
    struct Node *next;
} Node;


typedef struct {
    Node *head;

    int (*compare)(const void *, const void *);

    void (*freeData)(void *);
} SortedList;

SortedList *initializeList(int (*compare)(const void *, const void *), void (*freeData)(void *));

Node* createNode(void *data);

void insert(SortedList *list, void *data);

void printList(SortedList *list, void (*printData)(const void *));

void freeList(SortedList *list);



#endif /* MMN14_SORTED_LIST_H */

