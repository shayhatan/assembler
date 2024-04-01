//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_LABELS_TABLE_H
#define ASSEMBLER_LABELS_TABLE_H

#include "data_structures/linked_list/list.h"
#include "parsers/parse_types.h"

/**
 * allocates a map
 */
void labelsTableInit();

/**
 * de-allocates a map
 */
void labelsTableDispose();

/* adds a new label */
int setLabel(char *label, entry newEntry, bool create_only);

/* increments an existing label's words counter by 1 */
int incrementLabelWordsCounter(char *label);

int updateDataLabels(unsigned int IC);

int bulkAddExternalOperands(Arguments *args_container, bool create_only);

entry *get_data(char *label);

void printLabelsTable();

#endif //ASSEMBLER_LABELS_TABLE_H
