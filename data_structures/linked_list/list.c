//
// Created by User on 17/03/2024.
//

#include "stdbool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


enum ListResult setNodeValue(Node *node, void *value, DeleteFn freeElement) {
    if (node == NULL) {
        return LIST_NULL_NODE;
    }
    if (node->value != NULL) {
        freeElement(node->value);
    }
    node->value = malloc(sizeof(*value));
    if (node->value == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    memcpy(node->value, value, sizeof(*value));
    return LIST_SUCCESS;
}

enum ListResult listEnqueue(List list, void *value) {
    Node *newNode = NULL;
    enum ListResult status;
    if (list == NULL) {
        return LIST_NOT_INITIALIZED;
    }
    newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) {
        return LIST_OUT_OF_MEMORY;
    }
    status = setNodeValue(newNode, value, list->freeElement);
    if (status != LIST_SUCCESS) {
        return status;
    }

    newNode->previous = NULL;

    if (list->root == NULL) {
        list->root = newNode;
        newNode->next = NULL;
        newNode->value = value;
        return LIST_SUCCESS;
    }
    newNode->next = list->root;
    list->root->previous = newNode;
    list->root = newNode;
    return LIST_SUCCESS;
}

enum ListResult listPush(List list, void *value) {
    Node *newNode = NULL;
    Node *last = NULL;
    enum ListResult status;
    if (list == NULL) {
        return LIST_NOT_INITIALIZED;
    }
    newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) {

        return LIST_NULL_NODE;
    }
    status = setNodeValue(newNode, value, list->freeElement);
    if (status != LIST_SUCCESS) {
        return status;
    }

    newNode->next = NULL;

    if (list->root == NULL) {
        list->root = newNode;
        return LIST_SUCCESS;
    }
    last = listGetLast(list);
    last->next = newNode;
    newNode->previous = last;
    return LIST_SUCCESS;
}

enum ListResult listSetNth(List list, unsigned int index, void *value) {
    Node *nthNode = listGetNth(list, index);
    enum ListResult status;
    status = setNodeValue(nthNode, value, list->freeElement);
    if (status != LIST_SUCCESS) {
        return status;
    }
    return LIST_SUCCESS;
}

Node *listGetFirst(List list) {
    if (list == NULL) {
        return NULL;
    }
    return list->root;
}

Node *listGetLast(List list) {
    Node *current = NULL;
    if (list == NULL || list->root == NULL) {
        return NULL;
    }
    current = list->root;
    while (current->next != NULL) { current = current->next; }
    return current;
}

Node *listGetNth(List list, unsigned int index) {
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

Node *listSearch(List list, search_function callback, void *comparedData) {
    Node *current = listGetFirst(list);
    while (current && callback(current->value, comparedData) == false) {
        current = current->next;
    }
    return current;
}

enum ListResult listDeleteNth(List list, unsigned int index) {
    Node *nthNode = listGetNth(list, index);
    listDeleteNode(list, nthNode);
}

enum ListResult listDeleteNode(List list, Node *node) {
    if (node == NULL) {
        return LIST_ERROR;
    }
    if (node->previous) {
        (node->previous)->next = node->next;
    }
    if (node->next) {
        (node->next)->previous = node->previous;
    }

    /* listDispose of data */
    list->freeElement(node->value);
    free(node->value);
    free(node);
}

enum ListResult listDispose(List list) {
    Node *current;
    Node *next;
    if (list == NULL) return LIST_NOT_INITIALIZED;

    current = (list)->root;
    while (current != NULL) {
        next = current->next;

        /* listDispose of data */
        list->freeElement(current->value);

        current = next;
    }
    free(list);
}

int listIterate(List list, iterator_function callback) {
    unsigned int index = 0;
    Node *current = listGetFirst(list);
    while (current != NULL) {
        if (!callback(index++, current->value)) {
            return false;
        }
        current = current->next;
    }
    return true;
}

int listLength(List list) {
    int i = 0;
    Node *current = listGetFirst(list);
    while (current != NULL) {
        current = current->next;
        i++;
    }
    return i;
}

List listCreate(DeleteFn freeElement) {
    List list = malloc(sizeof(*list));
    list->root = NULL;
    list->freeElement = freeElement;
    return list;
}