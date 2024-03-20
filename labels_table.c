//
// Created by User on 17/03/2024.
//

#include <stdlib.h>
#include <string.h>
#include "./labels_table.h"
#include "parsers/types.h"
#include "logs/utils.h"
#include "data_structures/map/map.h"

Map labels_table;
Map *ptr;


MapDataElement copyElement(MapDataElement existing) {
    entry *clone = malloc(sizeof(entry));
    entry *existingEntry = existing;
    clone->classification = strdup(existingEntry->classification);
    clone->wordsCounter = existingEntry->wordsCounter;
    clone->value = existingEntry->value;
    return clone;
}

MapKeyElement copyKeyElement(MapKeyElement existing) {
    char *clone;

    return strdup((char *) existing);
}

/** Type of function for deallocating a data element of the map */
void cleanMapDataElements(MapDataElement disposable) {
    entry *disposedEntry = disposable;
    free(disposedEntry->classification);
}

/** Type of function for deallocating a key element of the map */
void cleanMapKeyElements(MapKeyElement disposable) {
}

int compareKeyElements(MapKeyElement key1, MapKeyElement key2) {
    char *s1 = key1;
    char *s2 = key2;
    return strcmp(s1, s2) == 0;
}

void init() {
    labels_table = mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                             compareKeyElements);
    ptr = &labels_table;
}

void deleteLabelDataCallback(void *value) {
    entry *valueAsEntry = value;
    free(valueAsEntry->classification);
}

void dispose() {
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


int updateDataLabels() {
    return 1;
}

entry *get_data(char *label) {
    return mapGet(labels_table, label);
}
