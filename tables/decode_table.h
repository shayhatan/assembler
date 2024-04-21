#ifndef ASSEMBLER_DECODE_TABLE_H
#define ASSEMBLER_DECODE_TABLE_H

#include "../data_structures/map/map.h"
#include "../words/types.h"
#include <stdio.h>

void wordsMapInit ();
void wordsMapDispose();

MapResult addWord(int address, word *word);

MapResult wordUpdateDecode(int IC);

MapIterationResult getNextLine(char* result);

bool isEmptyWordsMap();

int writeWordsMap(FILE* ob_file);
#endif /*ASSEMBLER_DECODE_TABLE_H*/
