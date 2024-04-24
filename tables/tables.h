#ifndef ASSEMBLER_TABLES_H
#define ASSEMBLER_TABLES_H
#include "decode_table.h"
#include "externals_table.h"
#include "labels_table.h"


/* Define the Tables structure */
typedef struct {
    Map externals_map;
    Map words_map;
    Map labels_table;
} Tables;

void tablesInit(Tables *tables);

void tablesDispose(Tables *tables);

#endif /*ASSEMBLER_TABLES_H*/
