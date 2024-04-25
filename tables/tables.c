#include "tables.h"

MapResult tablesInit(Tables *tables) {
    tables->externals_map = externalsMapCreate();
    if (!tables->externals_map) {
        return MAP_OUT_OF_MEMORY;
    }
    tables->words_map = wordsMapCreate();
    if (!tables->words_map) {
        wordsMapDispose(tables->externals_map);
        return MAP_OUT_OF_MEMORY;
    }
    tables->labels_table = labelsTableCreate();
    if (!tables->labels_table) {
        wordsMapDispose(tables->words_map);
        wordsMapDispose(tables->externals_map);
        return MAP_OUT_OF_MEMORY;
    }

    return MAP_SUCCESS;
}

void tablesDispose(Tables *tables) {
    externalsMapDispose(tables->externals_map);
    wordsMapDispose(tables->words_map);
    labelsTableDispose(tables->labels_table);
}

int getNodesNumber(Tables *tables) {
    return mapGetSize(tables->externals_map) + mapGetSize(tables->words_map) + mapGetSize(tables->labels_table);
}