#ifndef ASSEMBLER_DECODE_TABLE_H
#define ASSEMBLER_DECODE_TABLE_H

#include "../data_structures/map/map.h"
#include "../words/types.h"

void wordsMapInit ();
void wordsMapDispose();

MapResult addWord(int address, word *word);
void printWordsMap(void);

MapResult wordUpdateDecode(int IC);

MapIterationResult getNextLine(char* result);

bool isEmptyWordsMap();

#endif /*ASSEMBLER_DECODE_TABLE_H*/
