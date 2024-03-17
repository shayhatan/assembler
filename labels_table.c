//
// Created by User on 17/03/2024.
//

#include <stdlib.h>
#include "./labels_table.h"
#include "./data_structures/table/utils.h"
#include "parsers/types.h"
#include "logs/utils.h"

table labels_table;
table *ptr = &labels_table;

void init() {
    init_table(&labels_table);
}

void deleteLabelDataCallback(void *value) {
    entry *valueAsEntry = value;
    free(valueAsEntry->classification);
}

void dispose() {
    dispose_table(&ptr, deleteLabelDataCallback);
}

int addLabel(char *label, entry newEntry) {
    newEntry.wordsCounter = 0;

    if (getValue(ptr, label) == NULL) {
        setValue(ptr, label, &newEntry);
        return 0; /* success */
    }

    log_error("Cannot add label, label %s already exists", label);
    return 1; /* already exists */
}

int incrementLabelWordsCounter(char *label) {
    entry *existingEntry = getValue(ptr, label);
    if (existingEntry == NULL) {
        log_error("label %s does not exist", label);
        return 1; /* argument out of range */;
    }
    existingEntry->wordsCounter += 1;
    return 0; /* success */
}

entry *get_data(char *label) {
    return getValue(ptr, label);
}
