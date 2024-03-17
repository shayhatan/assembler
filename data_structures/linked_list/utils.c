//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

void addFirst(list *list, void *value) {
    node *newNode = NULL;
    if (*list == NULL) {
        return;
    }
    newNode = (node *) malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Out of memory");
        exit(0);
        return;
    }
    newNode->value = value;
    newNode->previous = NULL;
    if (list->root == NULL) {
        list->root = newNode;
        newNode->next = NULL;
        newNode->value = value;
        return;
    }
    newNode.next = list->root;
    list->root->previous = newNode;
    list->root = newNode;
}

void addLast(list *list, void *value) {
    node *newNode = NULL;
    node *last = NULL;
    if (*list == NULL) {
        return;
    }
    newNode = (node *) malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Out of memory");
        exit(0);
        return;
    }
    newNode->value = value;
    newNode->next = NULL;

    if (list->root == NULL) {
        list->root = newNode;
        return;
    }
    last = getLast(list);
    last->next = newNode;
    newNode->previous = last;
}

void *setNth(list *list, unsigned int index, void *value) {
    node *nthNode = getNth(list, index);
    void *currentData = nthNode->value;
    nthNode->value = value;
    return currentData;
}

node *getFirst(list *list) {
    if (list == NULL) {
        return NULL;
    }
    return list->root;
}

node *getLast(list *list) {
    node *current = NULL;
    if (list == NULL || list->root == NULL) {
        return NULL;
    }
    current = list->root;
    while (current.next != NULL && current = current.next);
    return current;
}

node *getNth(list *list, unsigned int index) {
    node *current = NULL;
    int currentIndex = 0;
    if (list == NULL || list->root == NULL) {
        return NULL;
    }
    current = list->root;
    while (current.next != NULL && current = current.next) {
        if (currentIndex == index) {
            return current;
        }
        currentIndex++;
    }
    /* index was out of range */
    return NULL;
}

void *deleteNth(list *list, unsigned int index, delete_function callback) {
    node *nthNode = getNth(list, index);
    if (!nthNode) {
        return NULL;
    }
    temp = nthNode->previous;
    if (nthNode->previous) {
        (nthNode->previous)->next = nthNode->next;
    }
    if (nthNode->next) {
        (nthNode->next)->previous = nthNode->previous;
    }

    /* dispose of data */
    callback(nthNode->value);
    free(nthNode);
}

void dispose(list **listPtr, delete_function callback) {
    node *current;
    node *next;
    if (listPtr == NULL || *listPtr == NULL) return;

    current = (*listPtr)->root;
    while (current != NULL) {
        next = current->next;

        /* dispose of data */
        callback(current->value);
        free(current);

        current = next;
    }
}

void iterate(list *list, iterator_function callback) {
    unsigned int index = 0;
    node *current = getFirst(&list);
    while (current != NULL) {
        callback(index++, current->value);
        current = current->next;
    }
}
