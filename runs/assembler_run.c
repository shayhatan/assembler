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


void cleanup(void);

void assemblerRun(char *files[], int index) {
    char am_file[81] = "", ob_file[81] = "", ex_file[81] = "", ent_file[81] = ""; /*Need to  handle errors properly*/
    int run_result = PARSE_SUCCESS;
    FILE *source_file = NULL, *obj_file = NULL, *ext_file = NULL, *entries_file = NULL;
    
    if (!preCompile(files, am_file, index)) {
        perror("Error precomiping");
        return;
    }

    source_file = fopen(am_file, "r");
    if (!source_file) {
        exit(-1);
    }

    labelsTableInit();
    wordsMapInit();
    externalsMapInit();

    run_result = run(source_file);

    if (run_result == OUT_OF_MEMORY) {
        cleanup();
        exit(run_result);
    }

    printLabelsTable();
    fseek(source_file, 0, SEEK_SET);
    run_result = secondRun(source_file);
    if (run_result != PARSE_SUCCESS) {
        cleanup();
        exit(run_result);
    }

    generateOutputFileName(files[index], ob_file, ".ob");
    obj_file = fopen(ob_file, "w");
    if (!obj_file) {
        perror("Error opening files");
        cleanup();
        exit(-1);
    }
    writeWordsMap(obj_file);
    fclose(obj_file);
    

    if (!isEmptyExternals()) {
        generateOutputFileName(files[index], ex_file, ".ext");
        ext_file = fopen(ex_file, "w");
        if (ext_file == NULL) {
            perror("Error opening files");
            cleanup();
            exit(-1);
        }
        writeExternals(ext_file);
        fclose(ext_file);
    }
    if (hasAnyDotEntryLabel()) {
        generateOutputFileName(files[index], ent_file, ".ent");
        entries_file = fopen(ent_file, "w");
        if (entries_file == NULL) {
            perror("Error opening files");
            cleanup();
            exit(-1);
        }
        writeEntriesFile(entries_file);
        fclose(entries_file);
    }


    printf("Run finished with status %d\n", run_result);
    cleanup();
}

void cleanup() {
    externalsMapDispose();
    wordsMapDispose();
    labelsTableDispose();
}
