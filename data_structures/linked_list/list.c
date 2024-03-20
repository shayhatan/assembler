//
// Created by User on 17/03/2024.
//

#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void setNodeValue(Node *node, void *value) {
    if (node->value != NULL) {
        free(node->value);
    }
    node->value = malloc(sizeof(*value));
    if (node->value == NULL) {
        /* out of memory */
        exit(1000);
    }
    memcpy(node->value, value, sizeof(*value));
}

void addFirst(List list, void *value) {
    Node *newNode = NULL;
    if (list == NULL) {
        return;
    }
    newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Out of memory");
        exit(0);
        return;
    }
    setNodeValue(newNode, value);

    newNode->previous = NULL;

    if (list->root == NULL) {
        list->root = newNode;
        newNode->next = NULL;
        newNode->value = value;
        return;
    }
    newNode->next = list->root;
    list->root->previous = newNode;
    list->root = newNode;
}

void addLast(List list, void *value) {
    Node *newNode = NULL;
    Node *last = NULL;
    if (list == NULL) {
        return;
    }
    newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Out of memory");
        exit(0);
        return;
    }
    setNodeValue(newNode, value);

    newNode->next = NULL;

    if (list->root == NULL) {
        list->root = newNode;
        return;
    }
    last = getLast(list);
    last->next = newNode;
    newNode->previous = last;
}

void *setNth(List list, unsigned int index, void *value) {
    Node *nthNode = getNth(list, index);
    void *currentData = nthNode->value;
    setNodeValue(nthNode, value);
    return currentData;
}

Node *getFirst(List list) {
    if (list == NULL) {
        return NULL;
    }
    return list->root;
}

Node *getLast(List list) {
    Node *current = NULL;
    if (list == NULL || list->root == NULL) {
        return NULL;
    }
    current = list->root;
    while (current->next != NULL) { current = current->next; }
    return current;
}

Node *getNth(List list, unsigned int index) {
    Node *current = NULL;
    int currentIndex = 0;
    if (list == NULL || list->root == NULL) {
        return NULL;
    }
    current = list->root;
    while (current) {
        if (currentIndex == index) {
            return current;
        }
        current = current->next;
        currentIndex++;
    }
    /* index was out of range */
    return NULL;
}

Node *search(List list, search_function callback, void *comparedData) {
    Node *current = getFirst(list);
    while (current && callback(current->value, comparedData) == false) {
        current = current->next;
    }
    return current;
}

void deleteNth(List list, unsigned int index) {
    Node *nthNode = getNth(list, index);
    deleteNode(list, nthNode);
}

void deleteNode(List list, Node *node) {
    if (!node) {
        return;
    }
    if (node->previous) {
        (node->previous)->next = node->next;
    }
    if (node->next) {
        (node->next)->previous = node->previous;
    }

    /* dispose of data */
    list->freeElement(node->value);
    free(node->value);
    free(node);
}

void dispose(List listPtr) {
    Node *current;
    Node *next;
    if (listPtr == NULL) return;

    current = (listPtr)->root;
    while (current != NULL) {
        next = current->next;

        /* dispose of data */
        listPtr->freeElement(current->value);
        free(current->value);
        free(current);

        current = next;
    }
    free(listPtr);
}

void iterate(List list, iterator_function callback) {
    unsigned int index = 0;
    Node *current = getFirst(list);
    while (current != NULL) {
        callback(index++, current->value);
        current = current->next;
    }
}

int listLength(List list) {
    int i = 0;
    Node *current = getFirst(list);
    while (current != NULL) {
        current = current->next;
        i++;
    }
    return i;
}

List init_list(DeleteFn freeElement) {
    List list = malloc(sizeof(*list));
    list->root = NULL;
    list->freeElement = freeElement;
    return list;
}