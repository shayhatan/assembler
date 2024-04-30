#ifndef ASSEMBLER_LABELS_TABLE_H
#define ASSEMBLER_LABELS_TABLE_H

#include <stdio.h>
#include <stdbool.h>
#include "../parsers/parse_types.h"
#include "../data_structures/map/map.h"

/**
 * Deallocates memory associated with a labels table.
 *
 * @param labels_table The labels table to deallocate.
 */
void labelsTableDispose(Map labels_table);

/**
 * Adds a new label to the labels table or updates an existing one.
 *
 * @param label The label name.
 * @param newEntry The entry associated with the label.
 * @param create_only Indicates whether to create only and not update existing labels.
 * @param labels_table The labels table to modify.
 * @return MapResult indicating success or failure.
 */
MapResult setLabel(char *label, Entry newEntry, bool create_only, Map labels_table);

/**
 * Increments the word count of an existing label in the labels table.
 *
 * @param label The label name.
 * @param labels_table The labels table containing the label.
 * @return The updated word count of the label.
 */
int incrementLabelWordsCounter(char *label, Map labels_table);

/**
 * Updates data labels in the labels table with the given instruction counter.
 *
 * @param IC The instruction counter value.
 * @param labels_table The labels table to update.
 * @return The number of data labels updated.
 */
int updateDataLabels(int IC, Map labels_table);

/**
 * Adds external operands to the labels table in bulk.
 *
 * @param args_container The container for arguments.
 * @param create_only Indicates whether to create only and not update existing labels.
 * @param labels_table The labels table to add external operands to.
 * @return MapResult indicating success or failure.
 */
MapResult bulkAddExternalOperands(Arguments *args_container, bool create_only, Map labels_table);

/**
 * Retrieves the entry associated with a label from the labels table.
 *
 * @param label The label name.
 * @param labels_table The labels table to search.
 * @return Pointer to the entry associated with the label, or NULL if not found.
 */
Entry *getEntry(char *label, Map labels_table);

/**
 * Prints the contents of the labels table.
 *
 * @param labels_table The labels table to print.
 */
void printLabelsTable(Map labels_table);

/**
 * Sets an entry label in the labels table.
 *
 * @param label The label name.
 * @param labels_table The labels table to modify.
 * @param has_dot_ent Indicates whether the label has a '.ent' directive.
 * @return MapResult indicating success or failure.
 */
MapResult setEntryLabel(char *label, Map labels_table, bool *has_dot_ent);

/**
 * Retrieves the constant value associated with a label from the labels table.
 *
 * @param label The label name.
 * @param result Pointer to store the constant value.
 * @param labels_table The labels table to search.
 * @return MapResult indicating success or failure.
 */
MapResult getConstantByLabel(char *label, unsigned int *result, Map labels_table);

/**
 * Retrieves the data counter and instruction counter values from the labels table.
 *
 * @param buffer The buffer to store the result.
 * @param labels_table The labels table containing the counters.
 * @param IC The instruction counter value.
 * @param DC The data counter value.
 */
void getDCAndIC(char buffer[81], Map labels_table, int IC, int DC);

/**
 * Writes entries from the labels table to a file.
 *
 * @param ent_file The file pointer to write to.
 * @param labels_table The labels table containing the entries.
 * @return The number of entries written to the file.
 */
int writeEntriesFile(FILE *ent_file, Map labels_table);

/**
 * Creates a new labels table.
 *
 * @return The newly created labels table.
 */
Map labelsTableCreate(void);

#endif /*ASSEMBLER_LABELS_TABLE_H*/
