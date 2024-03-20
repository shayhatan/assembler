//
// Created by User on 17/03/2024.
//

#include <stdlib.h>
#include <string.h>
#include "./labels_table.h"
#include "parsers/types.h"
#include "logs/utils.h"
#include "data_structures/map/map.h"
#include "list.h"
#include "factory.h"

static Map labels_table;


MapDataElement copyElement(MapDataElement existing) {
    entry *clone = malloc(sizeof(entry));
    entry *existingEntry = existing;
    clone->classification = strdup(existingEntry->classification);
    clone->wordsCounter = existingEntry->wordsCounter;
    clone->value = existingEntry->value;
    return clone;
}

MapKeyElement copyKeyElement(MapKeyElement existing) {
    return strdup((char *) existing);
}

/** Type of function for deallocating a data element of the map */
void cleanMapDataElements(MapDataElement disposable) {
    entry *disposedEntry = disposable;
    free(disposedEntry->classification);
}

/** Type of function for deallocating a key element of the map */
void cleanMapKeyElements(MapKeyElement _) {
}

int compareKeyElements(MapKeyElement key1, MapKeyElement key2) {
    char *s1 = key1;
    char *s2 = key2;
    return strcmp(s1, s2) == 0;
}

void init() {
    labels_table = mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                             compareKeyElements);
}

void deleteLabelDataCallback(void *value) {
    entry *valueAsEntry = value;
    free(valueAsEntry->classification);
}

void disposeLabelsTable() {
    mapDestroy(labels_table);
}

int addLabel(char *label, entry newEntry) {
    newEntry.wordsCounter = 0;

    if (mapContains(labels_table, label)) {
        mapPut(labels_table, label, &newEntry);
        return 0; /* success */
    }
    log_error("Cannot add label, label %s already exists", label);
    return 1; /* already exists */
}

int incrementLabelWordsCounter(char *label) {
    entry *existingEntry = mapGet(labels_table, label);
    if (existingEntry == NULL) {
        log_error("label %s does not exist", label);
        return 1; /* argument out of range */;
    }
    existingEntry->wordsCounter += 1;
    return 0; /* success */
}


int bulkAddExternalOperands(List o) {
    Node *current = getFirst(o);
    while (current != NULL) {
        Operand operand = current->value;
        /* todo: we might be adding the same label multiple times, perhaps a List is more appropriate? */
        addLabel(operand, createEntry(DOT_EXTERNAL, -1));
        current = current->next;
    }
}


int updateDataLabels(unsigned int IC) {
    entry *currentEntry;
    char *currentLabel = mapGetFirst(labels_table);
    /* no labels */
    if (currentLabel == NULL) {
        return 0;
    }
    currentEntry = mapGet(labels_table, currentLabel);
    if (strcmp(currentEntry->classification, DOT_DATA) == 0) {
        currentEntry->wordsCounter += IC + 100;
    }
    free(currentLabel);
    while (mapGetNext(labels_table) != NULL) {
        currentLabel = mapGetNext(labels_table);
        currentEntry = mapGet(labels_table, currentLabel);
        if (strcmp(currentEntry->classification, DOT_DATA) == 0) {
            currentEntry->wordsCounter += IC + 100;
        }
        free(currentLabel);
    }

    mapGetFirst(labels_table);

    return 1;
}

entry *get_data(char *label) {
    return mapGet(labels_table, label);
}
