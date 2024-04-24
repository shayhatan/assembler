#ifndef ASSEMBLER_LABELS_TABLE_H
#define ASSEMBLER_LABELS_TABLE_H

#include <stdio.h>
#include <stdbool.h>
#include "../parsers/parse_types.h"
#include "../data_structures/map/map.h"

extern unsigned int IC, DC;

/**
 * allocates a map
 */
void labelsTableInit();

/**
 * de-allocates a map
 */
void labelsTableDispose(Map labels_table);

/* adds a new label */
MapResult setLabel(char *label, entry newEntry, bool create_only, Map labels_table);

/* increments an existing label's words_map counter by 1 */
int incrementLabelWordsCounter(char *label, Map labels_table);

int updateDataLabels(unsigned int IC, Map labels_table);

MapResult bulkAddExternalOperands(Arguments *args_container, bool create_only, Map labels_table);

entry *getEntry(char *label, Map labels_table);

void printLabelsTable(Map labels_table);

MapResult setEntryLabel(char *label, Map labels_table);

MapResult getConstantByLabel(char *label, unsigned int *result, Map labels_table);

void getDCAndIC(char buffer[81], Map labels_table, int IC, int DC);

bool hasAnyDotEntryLabel(void);

int writeEntriesFile(FILE *ent_file, Map labels_table);

Map labelsTableCreate();

#endif /*ASSEMBLER_LABELS_TABLE_H*/
