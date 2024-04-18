/*
 Created by User on 31/03/2024.
*/

#include <stdio.h>
#include "runs/first_run.h"
#include "tables/labels_table.h"
#include "tables/decode_table.h"
#include "runs/second_run.h"
#include "tables/externals_table.h"


void cleanup(FILE *source_file);

int main() {
    int run_result = -3; /* undefined */

    FILE *source_file = fopen("C:\\Documents\\repos\\uni\\programming_lab\\assembler\\inputs\\h1.asm", "r");

    if (source_file == NULL) return -1;

    labelsTableInit();
    wordsMapInit();
    externalsMapInit();

    run_result = run(source_file);

    /* first run has finished successfully */
    if (run_result == OUT_OF_MEMORY) {
        cleanup(source_file);
        return OUT_OF_MEMORY;
    }

    printLabelsTable();
    fseek(source_file, 0, SEEK_SET);
    run_result = secondRun(source_file);
    printWordsMap();
    printExternals();

    printf("run finished with status %d\n", run_result);

    cleanup(source_file);

    return 1;
}

void cleanup(FILE *source_file) {
    externalsMapDispose();
    wordsMapDispose();
    labelsTableDispose();
    fclose(source_file);
}
