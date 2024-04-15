#include "sorted_list.h"
#include "../../helper/helper.h"

Node* createNode(void *data) {
    Node* new_node = allocateMemory(sizeof(Node));
    /*change to handleMemory function*/
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/* Function to initialize the sorted list */
SortedList *initializeList(int (*compare)(const void *, const void *), void (*freeData)(void *)) {

    SortedList *list = allocateMemory(sizeof(SortedList));
    if (list == NULL) {
        return NULL;
    }

    list->head = NULL;
    list->compare = compare;
    list->freeData = freeData;
    return list;
}

/* Function to insert data into the sorted list */
void insert(SortedList *list, void *data) {
    Node* newNode = createNode(data);
    Node* current = list->head;
    Node* prev = NULL;

    while (current != NULL && (list->compare)(current->data, data) < 0) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        newNode->next = list->head;
        list->head = newNode;
    } else {
        prev->next = newNode;
        newNode->next = current;
    }
}

/* Function to print the sorted list */
void printList(SortedList *list, void (*printData)(const void *)) {
    Node* current = list->head;
    while (current != NULL) {
        printData(current->data);
        printf(" -> ");
        current = current->next;
    }
    printf("NULL\n");
}

/* Function to free the memory allocated for the sorted list */
void freeList(SortedList *list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        if (list->freeData != NULL)
            list->freeData(current->data);
        free(current);
        current = next;
    }
    free(list);
}

