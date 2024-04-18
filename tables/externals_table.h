#ifndef ASSEMBLER_EXTERNALS_TABLE_H
#define ASSEMBLER_EXTERNALS_TABLE_H

#include "../data_structures/map/map.h"

void externalsMapInit();

void externalsMapDispose();

MapResult addExternal(int address, char *label);

void printExternals();

#endif /*ASSEMBLER_EXTERNALS_TABLE_H*/
