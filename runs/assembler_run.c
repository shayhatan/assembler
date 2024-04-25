#include "assembler_run.h"
#include <stdio.h>
#include "../runs/first_run.h"
#include "../runs/second_run.h"
#include "../precomplie/precompile.h"
#include "../utils/file_manger.h"
#include <stdlib.h>
#include "assembler_types.h"
#include "logs/logging_utils.h"


/*What's left?
 * errors final touch
 * in assemblerRun (here) refine the way we handle files
 * eliminate magic numbers such as 81
 * */

void assemblerRun(char *files[], int index) {
    char am_file[81] = "", ob_file[81] = "", ex_file[81] = "", ent_file[81] = ""; /*Need to  handle errors properly*/
    ParseResult current_run_result = PARSE_SUCCESS, overall_run_result = PARSE_SUCCESS;
    FILE *source_file = NULL, *obj_file = NULL, *ext_file = NULL, *entries_file = NULL;
    Assembler assembler;

    if (assemblerInit(&assembler) == MAP_OUT_OF_MEMORY) {
        logError("Out of memory during initialization\n");
        exit(-1);
    }

    if (!preCompile(files, am_file, index)) {
        logError("Error pre-compiling\n");
        return;
    }

    source_file = fopen(am_file, "r");
    if (!source_file) {
        assemblerDispose(&assembler);
        exit(-1);
    }

    current_run_result = run(source_file, &assembler);
    if (current_run_result != PARSE_SUCCESS) {
        overall_run_result = current_run_result;
    }

    if (current_run_result == OUT_OF_MEMORY) {
        assemblerDispose(&assembler);
        fclose(source_file);
        exit(current_run_result);
    }

    printLabelsTable(assembler.tables->labels_table);
    fseek(source_file, 0, SEEK_SET);
    current_run_result = secondRun(source_file, &assembler);

    fclose(source_file); /* close am file */

    if (current_run_result != PARSE_SUCCESS || overall_run_result != PARSE_SUCCESS) {
        assemblerDispose(&assembler);
        exit(current_run_result);
    }

    generateOutputFileName(files[index], ob_file, ".ob");
    obj_file = fopen(ob_file, "w");
    if (!obj_file) {
        logError("Error opening files\n");
        assemblerDispose(&assembler);
        exit(-1);
    }
    writeWordsMap(obj_file, assembler.tables->words_map, assembler.tables->labels_table, assembler.IC, assembler.DC);
    fclose(obj_file);


    if (!isEmptyExternals(assembler.tables->externals_map)) {
        generateOutputFileName(files[index], ex_file, ".ext");
        ext_file = fopen(ex_file, "w");
        if (ext_file == NULL) {
            logError("Error opening files\n");
            assemblerDispose(&assembler);
            exit(-1);
        }
        writeExternals(ext_file, assembler.tables->externals_map);
        fclose(ext_file);
    }
    if (assembler.has_dot_ent) {
        generateOutputFileName(files[index], ent_file, ".ent");
        entries_file = fopen(ent_file, "w");
        if (entries_file == NULL) {
            logError("Error opening files\n");
            assemblerDispose(&assembler);
            exit(-1);
        }
        writeEntriesFile(entries_file, assembler.tables->labels_table);
        fclose(entries_file);
    }

    printf("Run finished with status %d\n", current_run_result);

    assemblerDispose(&assembler);
}
