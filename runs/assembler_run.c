#include "assembler_run.h"
#include <stdio.h>
#include "../runs/first_run.h"
#include "../tables/labels_table.h"
#include "../tables/decode_table.h"
#include "../runs/second_run.h"
#include "../tables/externals_table.h"
#include "../precomplie/precompile.h"
#include <stdlib.h>

void cleanup(FILE *source_file);

void assemblerRun(char *files[], int index) {
    char am_file[81] = "";
    int run_result = -3; /* undefined */
    FILE *source_file;

    if (!preCompile(files, am_file, index))
        return;

    source_file = fopen(am_file, "r");

    if (source_file == NULL)
        return;

    labelsTableInit();
    wordsMapInit();
    externalsMapInit();

    run_result = run(source_file);

    /* first run has finished successfully */
    if (run_result == OUT_OF_MEMORY) {
        cleanup(source_file);
        remove(am_file);
        exit(1);
    }

    printLabelsTable();
    fseek(source_file, 0, SEEK_SET);
    run_result = secondRun(source_file);
    /*Do we need any check for result here? @itay ?*/

    printWordsMap();
    printExternals();

    printf("run finished with status %d\n", run_result);

    cleanup(source_file);

}


void cleanup(FILE *source_file) {
    externalsMapDispose();
    wordsMapDispose();
    labelsTableDispose();
    fclose(source_file);
}
