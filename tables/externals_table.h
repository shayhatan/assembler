#ifndef ASSEMBLER_EXTERNALS_TABLE_H
#define ASSEMBLER_EXTERNALS_TABLE_H

#include "../data_structures/map/map.h"

void externalsMapInit();

void externalsMapDispose();

MapResult addExternal(int address, char *label);

bool isEmptyExternals();

void printExternals(FILE* ext_file);

#endif /*ASSEMBLER_EXTERNALS_TABLE_H*/
