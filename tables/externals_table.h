#ifndef ASSEMBLER_EXTERNALS_TABLE_H
#define ASSEMBLER_EXTERNALS_TABLE_H

#include "../data_structures/map/map.h"
#include <stdio.h>


void externalsMapDispose(Map externals_map);

MapResult addExternal(int address, char *label, Map externals_map);

bool isEmptyExternals(Map externals_map);

int writeExternals(FILE *ext_file, Map externals_map);

Map externalsMapCreate();

#endif /*ASSEMBLER_EXTERNALS_TABLE_H*/
