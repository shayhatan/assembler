#ifndef ASSEMBLER_LABELS_TABLE_H
#define ASSEMBLER_LABELS_TABLE_H

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
void labelsTableDispose();

/* adds a new label */
MapResult setLabel(char *label, entry newEntry, bool create_only);

/* increments an existing label's words_map counter by 1 */
int incrementLabelWordsCounter(char *label);

int updateDataLabels(unsigned int IC);

MapResult bulkAddExternalOperands(Arguments *args_container, bool create_only);

entry *getEntry(char *label);

void printLabelsTable(void);

MapResult setEntryLabel(char *label);

MapResult getConstantByLabel(char* label, unsigned int* result);

void getDCAndIC(char buffer[81]);

#endif /*ASSEMBLER_LABELS_TABLE_H*/
