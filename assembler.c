/*
 Created by User on 31/03/2024.
*/

#include <stdio.h>
#include "./first_run.h"
#include "labels_table.h"
#include "decode_table.h"
#include "second_run.h"


int main() {
    int run_result = -3; /* undefined */

    FILE *source_file = fopen("C:\\Documents\\repos\\uni\\programming_lab\\assembler\\inputs\\h2.asm", "r");
    if (source_file == NULL) return -1;

    labelsTableInit();
    wordsMapInit();

    run_result = run(source_file);

    /* first run has finished successfully */
    if (run_result == PARSE_SUCCESS) {
        printLabelsTable();
        fseek(source_file, 0, SEEK_SET);
        run_result = secondRun(source_file);
        printWordsMap();
    }


    printf("run finished with status %d", run_result);

    wordsMapDispose();
    labelsTableDispose();

    fclose(source_file);
    
    return 1;
}