//
// Created by User on 08/04/2024.
//

#ifndef ASSEMBLER_DECODE_TABLE_H
#define ASSEMBLER_DECODE_TABLE_H

#include "data_structures/map/map.h"
#include "words/types.h"

void wordsMapInit ();
void wordsMapDispose();

MapResult addWord(int address, word *word);
void printWordsMap();

#endif //ASSEMBLER_DECODE_TABLE_H
