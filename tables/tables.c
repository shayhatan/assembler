#include "tables.h"

void tablesInit(Tables *tables) {
    tables->externals_map = externalsMapCreate();
    /* @Itay in each step we need to check var is not null and i so to free prev allocations and exit(1)*/
    tables->words_map = wordsMapCreate();
    tables->labels_table = labelsTableCreate();
}


void tablesDispose(Tables *tables) {
    externalsMapDispose(tables->externals_map);
    wordsMapDispose(tables->words_map);
    labelsTableDispose(tables->labels_table);
}
