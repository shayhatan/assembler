/*
 Created by User on 17/03/2024.
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./labels_table.h"
#include "parsers/parse_types.h"
#include "logs/logging_utils.h"
#include "data_structures/map/map.h"
#include "factory.h"
#include "string_utils.h"

static Map labels_table;


MapDataElement copyElement(MapDataElement existing) {
    entry *clone = malloc(sizeof(entry));
    entry *existingEntry = existing;

    if (clone == NULL || existing == NULL) {
        return NULL;
    }
    clone->classification = allocatedDuplicateString(existingEntry->classification);
    clone->wordsCounter = existingEntry->wordsCounter;
    clone->value = existingEntry->value;
    return clone;
}

MapKeyElement copyKeyElement(MapKeyElement existing) {
    char *existing_str = (char *) existing;
    if (existing == NULL) return NULL;
    return allocatedDuplicateString(existing_str);
}

/** Type of function for deallocating a data element of the map */
void cleanMapDataElements(MapDataElement disposable) {
    entry *disposedEntry = disposable;
    free(disposedEntry->classification);
    free(disposable);
}

/** Type of function for deallocating a key element of the map */
void cleanMapKeyElements(MapKeyElement key) {
    free(key);
}

int compareKeyElements(MapKeyElement key1, MapKeyElement key2) {
    char *s1 = key1;
    char *s2 = key2;
    return strcmp(s1, s2);
}

void labelsTableInit() {
    if (labels_table != NULL) return;
    labels_table = mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                             compareKeyElements);
}

void labelsTableDispose() {
    mapDestroy(labels_table);
}

MapResult setLabel(char *label, entry newEntry, bool create_only) {
    newEntry.wordsCounter = 0;

    if (create_only && mapContains(labels_table, label)) {
        log_error("Cannot add label, label %s already exists\n", label);
        return MAP_ITEM_ALREADY_EXISTS; /* already exists */
    }
    return mapPut(labels_table, label, &newEntry);
}

int incrementLabelWordsCounter(char *label) {
    entry *existingEntry = mapGet(labels_table, label);
    if (existingEntry == NULL) {
        log_error("label %s does not exist\n", label);
        return 1; /* argument out of range */;
    }
    existingEntry->wordsCounter += 1;
    return 0; /* success */
}


MapResult bulkAddExternalOperands(Arguments *args_container, bool create_only) {
    int index = 0;
    MapResult status = MAP_SUCCESS;
    for (; index < args_container->args_count; index++) {
        status = setLabel(args_container->args[index], createEntry(DOT_EXTERNAL, -1), create_only);
        if (status != MAP_SUCCESS) {
            return status;
        }
    }
    return true;
}


int updateDataLabels(unsigned int IC) {
    entry *currentEntry;
    char* iter;
    MAP_FOREACH(char *, iter, labels_table) {
        if (iter == NULL) {
            log_error("Out of memory\n");
            return OUT_OF_MEMORY;
        }
        currentEntry = mapGet(labels_table, iter);
        if (strcmp(currentEntry->classification, DOT_DATA) == 0) {
            currentEntry->value += ((int) IC) + 100;
        }

        free(iter);
    }

    return MAP_SUCCESS;
}


void printLabelsTable() {
    entry *currentEntry = NULL;
    char* iter;
    if (labels_table == NULL) return;
    printf("============Labels table============\n");
    printf("Label\tValue\tClassification\tExtra Words\n");
    MAP_FOREACH(char*, iter, labels_table) {
        if (iter == NULL) {
            printf("====================================\n");
            break;
        }
        currentEntry = mapGet(labels_table, iter);
        if (currentEntry == NULL) {
            log_error("unreachable code had been reached");
            free(iter);
            return;
        }
        printf("%s\t%d\t%s\t%d\n", (char *) iter, currentEntry->value, currentEntry->classification,
               currentEntry->wordsCounter);


        free(iter);
    }

    printf("====================================\n");

}

enum MapResult_t setEntryLabel(char* label) {
    entry *labelEntry = NULL;

    if (labels_table == NULL) {
        return MAP_ERROR;
    }
    if (label == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    if (!mapContains(labels_table,label)) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    labelEntry = mapGet(labels_table, label);
    if (labelEntry == NULL) {
        return MAP_ERROR; /* shouldn't be possible as we have just verified that the label does exist */
    }

    labelEntry->isEntry = true;

    return MAP_SUCCESS;
}

entry *get_data(char *label) {
    return mapGet(labels_table, label);
}

MapResult getConstantByLabel(char* label, unsigned int* result) {
    entry *constant_entry = NULL;
    constant_entry =  mapGet(labels_table, label);
    if (constant_entry == NULL) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if (strcmp(constant_entry->classification, DOT_DEFINE) != 0) {
        return MAP_ERROR;
    }
    *result = (unsigned) constant_entry->value;
    return MAP_SUCCESS;
}
