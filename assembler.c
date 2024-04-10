/*
 Created by User on 31/03/2024.
*/

#include <stdio.h>
#include "./first_run.h"
#include "labels_table.h"


int main() {
    int run_result = -3; /* undefined */

    FILE *source_file = fopen("C:\\Documents\\repos\\uni\\programming_lab\\assembler\\inputs\\i1.asm", "r");
    if (source_file == NULL) return -1;
    run_result = run(source_file);

    /* first run has finished successfully */
    if (run_result == 0) {
        printLabelsTable();
    }

    printf("run finished with status %d", run_result);

    labelsTableDispose();
    fclose(source_file);
}