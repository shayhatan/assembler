//
// Created by User on 17/03/2024.
//

#include <string.h>
#include <stdbool.h>
#include "./types.h"
#include "../linked_list/types.h"
#include "../linked_list/utils.h"

bool tableSearchFunction (void* node_value, void* comparedData){
    key_value* kvPtr = (key_value*)node_value;
    char* comparedLabel = (char*)comparedData;
    return strcmp(kvPtr->label,comparedLabel);
}

void *getNodeByLabel(table *table, char *label) {
    if (table == NULL || table->list == NULL) {
        return NULL;
    }
    return search(table->list, tableSearchFunction)
};

void setValue(table *table, char *label, void *value) {
    node *kv_node = getNodeByLabel(table, label);
    if (kv_node != NULL) {
        ((key_value *) (kv_node->value))->value = value;
    }
}

void *getValue(table *table, char *label) {
    node* node = getNodeByLabel(table, label);
    if (node == NULL) {
        return NULL;
    }
    return node->value;
}


void *deleteKey(table *table, char *label, delete_function callback) {
    node * node = getNodeByLabel(table, label);
    deleteNode(node);
}

void dispose(table **table, delete_function callback) {
    dispose(&((**table).list), callback);
}


void iterate(table *table, iterator_function callback) {
    iterate(table->list, callback);
}
