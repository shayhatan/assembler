#ifndef ASSEMBLER_DECODE_TABLE_H
#define ASSEMBLER_DECODE_TABLE_H

#include "../data_structures/map/map.h"
#include "../words/types.h"
#include <stdio.h>

void wordsMapDispose(Map words_map);

MapResult addWord(int address, Word *word, Map words_map);

MapResult wordUpdateDecode(int IC, Map words_map);

MapIterationResult getNextLine(char *result, Map words_map);

int writeWordsMap(FILE *ob_file, Map words_map, Map labels_table, int IC, int DC);

Map wordsMapCreate();
#endif /*ASSEMBLER_DECODE_TABLE_H*/
