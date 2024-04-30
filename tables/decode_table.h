#ifndef ASSEMBLER_DECODE_TABLE_H
#define ASSEMBLER_DECODE_TABLE_H

#include "../data_structures/map/map.h"
#include "../words/types.h"
#include <stdio.h>
/**
 * Disposes of the memory allocated for a words map.
 *
 * @param words_map The words map to dispose of.
 */
void wordsMapDispose(Map words_map);

/**
 * Adds a word to the words map at the specified address.
 *
 * @param address The address to add the word to.
 * @param word Pointer to the word to add.
 * @param words_map The words map to add the word to.
 * @return MapResult indicating success or failure.
 */
MapResult addWord(int address, Word *word, Map words_map);

/**
 * Updates the decode of a word in the words map.
 *
 * @param IC The instruction counter value.
 * @param words_map The words map to update.
 * @return MapResult indicating success or failure.
 */
MapResult wordUpdateDecode(int IC, Map words_map);

/**
 * Gets the next output line from the words map.
 *
 * @param result Pointer to store the next line.
 * @param words_map The words map to iterate over.
 * @return MapIterationResult indicating success, end of map, or failure.
 */
MapIterationResult getNextLine(char *result, Map words_map);

/**
 * Writes the words map to an output file.
 *
 * @param ob_file The output file pointer.
 * @param words_map The words map to write.
 * @param labels_table The labels table associated with the words map.
 * @param IC The instruction counter value.
 * @param DC The data counter value.
 * @return The number of words written to the file.
 */
int writeWordsMap(FILE *ob_file, Map words_map, Map labels_table, int IC, int DC);

/**
 * Creates a new words map.
 *
 * @return The newly created words map.
 */
Map wordsMapCreate(void);

#endif /*ASSEMBLER_DECODE_TABLE_H*/
