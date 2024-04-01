//
// Created by User on 17/03/2024.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./labels_table.h"
#include "parsers/parse_types.h"
#include "logs/logging_utils.h"
#include "data_structures/map/map.h"
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
    return strcmp(s1, s2);
}

void labelsTableInit() {
    labels_table = mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                             compareKeyElements);
}

void labelsTableDispose() {
    mapDestroy(labels_table);
}

int setLabel(char *label, entry newEntry, bool create_only) {
    newEntry.wordsCounter = 0;

    if (create_only && mapContains(labels_table, label)) {
        log_error("Cannot add label, label %s already exists", label);
        return 1; /* already exists */
    }
    mapPut(labels_table, label, &newEntry);
    return 0; /* success */
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


int bulkAddExternalOperands(Arguments *args_container, bool create_only) {
    int index = 0;
    for (; index < args_container->args_count; index++) {
        setLabel(args_container->args[index], createEntry(DOT_EXTERNAL, -1), create_only);
    }
    return true;
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


void printLabelsTable() {
    entry *currentEntry;
    printf("============Labels table============\n");
    char *currentLabel = mapGetFirst(labels_table);
    /* no labels */
    if (currentLabel == NULL) {
        printf("====================================\n");
        return;
    }
    currentEntry = mapGet(labels_table, currentLabel);
    printf("Label\tValue\tClassification\tExtra Words\n");
    printf("%s\t%d\t%s\t%d\n", currentLabel, currentEntry->value, currentEntry->classification,
           currentEntry->wordsCounter);
    free(currentLabel);
    while (mapGetNext(labels_table) != NULL) {
        currentLabel = mapGetNext(labels_table);
        currentEntry = mapGet(labels_table, currentLabel);
        printf("%s\t%d\t%s\t%d\n", currentLabel, currentEntry->value, currentEntry->classification,
               currentEntry->wordsCounter);
        free(currentLabel);
    }

    mapGetFirst(labels_table);

    printf("====================================\n");

}

entry *get_data(char *label) {
    return mapGet(labels_table, label);
}
