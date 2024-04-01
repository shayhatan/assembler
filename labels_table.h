//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_LABELS_TABLE_H
#define ASSEMBLER_LABELS_TABLE_H

#include "data_structures/linked_list/list.h"
#include "parsers/parse_types.h"

/* adds a new label */
int addLabel(char *label, entry newEntry, bool create_only);

/* increments an existing label's words counter by 1 */
int incrementLabelWordsCounter(char *label);

int updateDataLabels(unsigned int IC);

int bulkAddExternalOperands(List o);

entry *get_data(char *label);

void disposeLabelsTable();

void printLabelsTable();

#endif //ASSEMBLER_LABELS_TABLE_H
