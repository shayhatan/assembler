//
// Created by User on 17/03/2024.
//

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "./types.h"
#include "../linked_list/list.h"

bool tableSearchFunction(void *node_value, void *comparedData) {
    key_value *kvPtr = (key_value *) node_value;
    char *comparedLabel = (char *) comparedData;
    return strcmp(kvPtr->label, comparedLabel) == 0;
}

void *getNodeByLabel(table *table, char *label) {
    if (table == NULL || table->list == NULL) {
        return NULL;
    }
    return search(table->list, tableSearchFunction, (void *) label);
};

void setValue(table *table, char *label, void *value) {
    key_value kv;
    Node *kv_node = getNodeByLabel(table, label);
    if (kv_node != NULL) {
        ((key_value *) (kv_node->value))->value = value;
        return;
    }
    /* inserts a new kv */

    /* strdup returns a dynamic allocated clone string */
    kv.label = strdup(label);

    kv.value = malloc(sizeof(*value));
    memcpy(kv.value, value, sizeof(*value));

    /* addLast will instantiate kv in dynamic memory, and it will use kv properties defined here */
    addLast(table->list, &kv);
}

void *getValue(table *table, char *label) {
    Node *node = getNodeByLabel(table, label);
    if (node == NULL) {
        return NULL;
    }
    return node->value;
}

void do_nothing(void *data) {}

void delete_key_value_props(key_value *kv, DeleteFn callback) {
    free(kv->label);
    callback(kv->value);
    free(kv->value);
}

void deleteKey(table *table, char *label, DeleteFn callback) {
    key_value *kv = NULL;
    Node *node = getNodeByLabel(table, label);
    if (node == NULL) {
        return;
    }
    kv = node->value;
    delete_key_value_props(kv, callback);
    /* kv will be freed within deleteNode */
    deleteNode(table->list, node);
}

void dispose_table(table **table, DeleteFn callback) {
    Node *current;
    Node *next;
    if (table == NULL || *table == NULL) {
        return;
    }

    current = (*table)->list->root;
    while (current != NULL) {
        next = current->next;

        /* dispose of data */
        delete_key_value_props(current->value, callback);
        /* delete kv */
        free(current->value);
        free(current);

        current = next;
    }
    free((*table)->list);
    free(*table);
}

void iterate_table(table *table, iterator_function callback) {
    iterate(table->list, callback);
}

void init_table(table *table) {
//    *table = (table*)malloc(sizeof(table));
};
