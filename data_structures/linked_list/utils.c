//
// Created by User on 17/03/2024.
//

#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "utils.h"

void setNodeValue(node* node, void* value){
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

void addFirst(list *list, void *value) {
    node *newNode = NULL;
    if (list == NULL) {
        return;
    }
    newNode = (node *) malloc(sizeof(node));
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

void addLast(list *list, void *value) {
    node *newNode = NULL;
    node *last = NULL;
    if (list == NULL) {
        return;
    }
    newNode = (node *) malloc(sizeof(node));
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

void *setNth(list *list, unsigned int index, void *value) {
    node *nthNode = getNth(list, index);
    void *currentData = nthNode->value;
    setNodeValue(nthNode, value);
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
    while (current->next != NULL) {current = current->next;}
    return current;
}

node *getNth(list *list, unsigned int index) {
    node *current = NULL;
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

node *search(list *list, search_function callback, void *comparedData) {
    node *current = getFirst(list);
    while (current && callback(current->value, comparedData) == false) {
        current = current->next;
    }
    return current;
}

void deleteNth(list *list, unsigned int index, delete_function callback) {
    node *nthNode = getNth(list, index);
    deleteNode(nthNode, callback);
}

void deleteNode(node *node, delete_function callback) {
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
    callback(node->value);
    free(node->value);
    free(node);
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
        free(current->value);
        free(current);

        current = next;
    }
    free(*listPtr);
}

void iterate(list *list, iterator_function callback) {
    unsigned int index = 0;
    node *current = getFirst(list);
    while (current != NULL) {
        callback(index++, current->value);
        current = current->next;
    }
}

int listLength(list *list) {
    int i =0;
    node *current = getFirst(list);
    while (current != NULL) {
        current = current->next;
        i++;
    }
    return i;
}

void init_list(list *list) {
    list = (list*)malloc(sizeof(list));
    list->root= NULL;
}