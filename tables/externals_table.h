#ifndef ASSEMBLER_EXTERNALS_TABLE_H
#define ASSEMBLER_EXTERNALS_TABLE_H

#include "../data_structures/map/map.h"
#include <stdio.h>

void externalsMapInit();

void externalsMapDispose();

MapResult addExternal(int address, char *label);

bool isEmptyExternals();

void writeExternals(FILE* ext_file);

#endif /*ASSEMBLER_EXTERNALS_TABLE_H*/
