#include <stdio.h>
#include <stdlib.h>
#include "assembler_run.h"
#include "../runs/first_run.h"
#include "../runs/second_run.h"
#include "../precompile/precompile.h"
#include "../utils/file_manager.h"
#include "../logs/logging_utils.h"

typedef int (*WriteFunction)(FILE*, Map);

bool tryCreateOutputFile(char *source_file_path, char* output_file_path, char* extension, Map map, WriteFunction writeFn) {
    FILE* file = NULL;
    generateOutputFileName(source_file_path, output_file_path, extension);
    setLogLineContext(-1, output_file_path, "OUTPUTS");
    file = fopen(output_file_path, "w");
    if (file == NULL) {
        logError("Error creating output file %s files\n", output_file_path);
        return false;
    }
    writeFn(file, map);
    fclose(file);
    return true;
}

void writeOutputs(char *file, Assembler *assembler) {
    char ob_file[81] = "", ex_file[81] = "", ent_file[81] = "";
    FILE *obj_file = NULL;


    generateOutputFileName(file, ob_file, ".ob");
    setLogLineContext(-1, ob_file, "OUTPUTS");
    obj_file = fopen(ob_file, "w");
    if (!obj_file) {
        logError("Error creating object file\n");
        assemblerDispose(assembler);
        exit(-1);
    }
    writeWordsMap(obj_file, (*assembler).tables->words_map, (*assembler).tables->labels_table, (int) (*assembler).IC, (int) (*assembler).DC);
    fclose(obj_file);

    /* try writing externals */
    if (!isEmptyExternals((*assembler).tables->externals_map)) {
        if (!tryCreateOutputFile(file, ex_file, ".ext", (*assembler).tables->externals_map, writeExternals)) {
            assemblerDispose(assembler);
            exit(-1);
        }
    }

    /* try writing entries */
    if ((*assembler).has_dot_ent) {
        if (!tryCreateOutputFile(file, ent_file, ".ent", (*assembler).tables->labels_table, writeEntriesFile)) {
            assemblerDispose(assembler);
            exit(-1);
        }
    }
}

void assemblerRun(char *files[], int index) {
    char am_file[81] = ""; /*Need to  handle errors properly*/
    ParseResult current_run_result = PARSE_SUCCESS, overall_run_result = PARSE_SUCCESS;
    FILE *source_file = NULL;
    Assembler assembler;

    if (assemblerInit(&assembler) == MAP_OUT_OF_MEMORY) {
        logError("Out of memory during initialization\n");
        exit(-1);
    }

    if (!preCompile(files, am_file, index)) {
        logError("Error pre-compiling\n");
        exit(-2);
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

    /* runs finished successfully, create output files */

    writeOutputs(files[index],  &assembler);

    printf("Compilation finished with status %d\n", overall_run_result);

    assemblerDispose(&assembler);
}
