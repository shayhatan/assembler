//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_LABELS_TABLE_H
#define ASSEMBLER_LABELS_TABLE_H

#include "parsers/types.h"

/* adds a new label */
int addLabel(char *label, entry newEntry);

/* increments an existing label's words counter by 1 */
int incrementLabelWordsCounter(char *label);

entry *get_data(char *label);

#endif //ASSEMBLER_LABELS_TABLE_H
