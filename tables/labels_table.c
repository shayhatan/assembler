#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "./labels_table.h"
#include "../logs/logging_utils.h"
#include "../utils/factory.h"
#include "../utils/string_utils.h"
#include "../utils/memory.h"


MapDataElement copyElement(MapDataElement existing) {
    Entry *clone = allocateMemory(sizeof(Entry));
    Entry *existing_entry = existing;

    if (clone == NULL || existing == NULL) {
        return NULL;
    }
    clone->classification = allocatedDuplicateString(existing_entry->classification);
    clone->words_counter = existing_entry->words_counter;
    clone->value = existing_entry->value;
    clone->is_entry = existing_entry->is_entry;
    return clone;
}

MapKeyElement copyKeyElement(MapKeyElement existing) {
    char *existing_str = (char *) existing;
    if (existing == NULL) return NULL;
    return allocatedDuplicateString(existing_str);
}

/** Type of function for deallocating a data element of the map */
void cleanMapDataElements(MapDataElement disposable) {
    Entry *disposed_entry = disposable;
    free(disposed_entry->classification);
    free(disposable);
}

/** Type of function for deallocating a key element of the map */
void cleanMapKeyElements(MapKeyElement key) {
    free(key);
}

int compareKeyElements(MapKeyElement key1, MapKeyElement key2) {
    char *s1 = key1;
    char *s2 = key2;
    int result = strcmp(s1, s2);
    if (result != 0) return -result;
    return result;
}

Map labelsTableCreate() {
    return mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                     compareKeyElements);
}


void labelsTableDispose(Map labels_table) {
    mapDestroy(labels_table);
}

MapResult setLabel(char *label, Entry newEntry, bool create_only, Map labels_table) {
    newEntry.words_counter = 0;

    if (create_only && mapContains(labels_table, label)) {
        logError("Cannot add label, label %s already exists\n", label);
        return MAP_ITEM_ALREADY_EXISTS; /* already exists */
    }
    return mapPut(labels_table, label, &newEntry);
}

int incrementLabelWordsCounter(char *label, Map labels_table) {
    Entry *existing_entry = mapGet(labels_table, label);
    if (existing_entry == NULL) {
        logError("label %s does not exist\n", label);
        return 1; /* argument out of range */;
    }
    existing_entry->words_counter += 1;
    return 0; /* success */
}


MapResult bulkAddExternalOperands(Arguments *args_container, bool create_only, Map labels_table) {
    int index = 0;
    MapResult status = MAP_SUCCESS;
    for (; index < args_container->args_count; index++) {
        status = setLabel(args_container->args[index], createEntry(DEF_DOT_EXTERNAL, -1), create_only, labels_table);
        if (status != MAP_SUCCESS) {
            return status;
        }
    }
    return true;
}


int updateDataLabels(int IC, Map labels_table) {
    Entry *current_entry;
    char *iter;
    MAP_FOREACH(char *, iter, labels_table) {
        if (iter == NULL) {
            logError("Out of memory\n");
            return OUT_OF_MEMORY;
        }
        current_entry = mapGet(labels_table, iter);
        if (strcmp(current_entry->classification, DEF_DOT_DATA) == 0) {
            current_entry->value += ((int) IC) + 100;
        }

        free(iter);
    }

    return MAP_SUCCESS;
}


void printLabelsTable(Map labels_table) {
    Entry *current_entry = NULL;
    char *iter = NULL;
    if (labels_table == NULL) return;
    printf("============Labels table============\n");
    printf("Label\tValue\tClassification\tExtra Words\tisEntry\n");
    MAP_FOREACH(char*, iter, labels_table) {
        if (iter == NULL) {
            printf("====================================\n");
            break;
        }
        current_entry = mapGet(labels_table, iter);
        if (current_entry == NULL) {
            logError("unreachable code had been reached");
            free(iter);
            return;
        }
        printf("%s\t%d\t%s\t%d\t%d\n", (char *) iter, current_entry->value, current_entry->classification,
               current_entry->words_counter, current_entry->is_entry);

        free(iter);
    }

    printf("====================================\n");
}

void getDCAndIC(char buffer[81], Map labels_table, int IC, int DC) {
    Entry *current_entry = NULL;
    char *iter;
    if (labels_table == NULL) return;
    MAP_FOREACH(char*, iter, labels_table) {
        if (iter == NULL) {
            break;
        }
        current_entry = mapGet(labels_table, iter);
        if (current_entry == NULL) {
            logError("unreachable code had been reached");
            free(iter);
            return;
        }

        free(iter);
    }

    sprintf(buffer, "%d\t%d", IC, DC);
}

MapResult setEntryLabel(char *label, Map labels_table, bool *has_dot_ent) {
    Entry *label_entry = NULL;

    if (labels_table == NULL) {
        return MAP_ERROR;
    }
    if (label == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    if (!mapContains(labels_table, label)) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }

    label_entry = mapGet(labels_table, label);
    if (label_entry == NULL) {
        return MAP_ERROR; /* shouldn't be possible as we have just verified that the label does exist */
    }

    label_entry->is_entry = true;
    *has_dot_ent = true;

    return MAP_SUCCESS;
}

Entry *getEntry(char *label, Map labels_table) {
    return mapGet(labels_table, label);
}

MapResult getConstantByLabel(char *label, unsigned int *result, Map labels_table) {
    Entry *constant_entry = NULL;
    constant_entry = mapGet(labels_table, label);
    if (constant_entry == NULL) {
        return MAP_ITEM_DOES_NOT_EXIST;
    }
    if (strcmp(constant_entry->classification, DEF_DOT_DEFINE) != 0) {
        return MAP_ERROR;
    }
    *result = (unsigned) constant_entry->value;
    return MAP_SUCCESS;
}


int writeEntriesFile(FILE *ent_file, Map labels_table) {
    char *iter;
    Entry *data = NULL;
    int size = 0;
    printf("=======ENTRY======");
    for (iter = mapGetFirst(labels_table); iter != NULL; iter = mapGetNext(labels_table)) {
        ++size;
        data = mapGet(labels_table, iter);
        if (data->is_entry) {
            printf("\n%s, %d\n", iter, data->value);
            fprintf(ent_file, "%s\t%d\n", iter, data->value);
        }
        free(iter);
    }
    return size;
}
