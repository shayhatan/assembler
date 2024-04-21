#include "assembler_run.h"
#include <stdio.h>
#include "../runs/first_run.h"
#include "../tables/labels_table.h"
#include "../tables/decode_table.h"
#include "../runs/second_run.h"
#include "../tables/externals_table.h"
#include "../precomplie/precompile.h"
#include "../utils/file_manger.h"
#include <stdlib.h>


void cleanup(FILE *source_file, FILE *obj_file, FILE *ext_file, bool obj_removed, bool ext_removed);

void assemblerRun(char *files[], int index) {
    char am_file[81] = "", ob_file[81] = "", ex_file[81] = ""; /*Need to add file of enrty and handle errors properly*/
    int run_result = -3;
    bool obj_removed = false, ext_removed = false;

    FILE *source_file = NULL, *obj_file = NULL, *ext_file = NULL;
    
    if (!preCompile(files, am_file, index)) {
        return;
    }

    generateOutputFileName(files[index], ob_file, ".ob");
    generateOutputFileName(files[index], ex_file, ".ext");

    source_file = fopen(am_file, "r");
    obj_file = fopen(ob_file, "w");
    ext_file = fopen(ex_file, "w");

    if (!source_file || !obj_file || !ext_file) {
        perror("Error opening files");
        cleanup(source_file, obj_file, ext_file, obj_removed, ext_removed);
        return;
    }

    labelsTableInit();
    wordsMapInit();
    externalsMapInit();

    run_result = run(source_file);

    if (run_result == OUT_OF_MEMORY) {
        cleanup(source_file, obj_file, ext_file, obj_removed, ext_removed);
        exit(1);
    }

    printLabelsTable();
    fseek(source_file, 0, SEEK_SET);
    run_result = secondRun(source_file);
    writeWordsMap(obj_file);
    writeExternals(ext_file);

    if (isEmptyWordsMap()) {
        fclose(obj_file);
        remove(ob_file);
        obj_removed = true;
    }

    if (isEmptyExternals()) {
        fclose(ext_file);
        remove(ex_file);
        ext_removed = true;
    }

    printf("Run finished with status %d\n", run_result);
    cleanup(source_file, obj_file, ext_file, obj_removed, ext_removed);
}

void cleanup(FILE *source_file, FILE *obj_file, FILE *ext_file, bool obj_removed, bool ext_removed) {
    externalsMapDispose();
    wordsMapDispose();
    labelsTableDispose();

    if (source_file)
        fclose(source_file);
    if (obj_file && !obj_removed)
        fclose(obj_file);
    if (ext_file && !ext_removed)
        fclose(ext_file);
}
