/*
 Created by User on 17/03/2024.
*/


#ifndef ASSEMBLER_LABELS_TABLE_H
#define ASSEMBLER_LABELS_TABLE_H

#include "parsers/parse_types.h"
#include "data_structures/map/map.h"

/**
 * allocates a map
 */
void labelsTableInit();

/**
 * de-allocates a map
 */
void labelsTableDispose();

/* adds a new label */
MapResult setLabel(char *label, entry newEntry, bool create_only);

/* increments an existing label's words_map counter by 1 */
int incrementLabelWordsCounter(char *label);

int updateDataLabels(unsigned int IC);

MapResult bulkAddExternalOperands(Arguments *args_container, bool create_only);

entry *get_data(char *label);

void printLabelsTable();

MapResult setEntryLabel(char *label);

MapResult getConstantByLabel(char* label, unsigned int* result);

#endif /*ASSEMBLER_LABELS_TABLE_H*/
