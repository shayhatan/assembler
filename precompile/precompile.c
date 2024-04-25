#include "./precompile.h"
#include "./macro/macros.h"
#include "./macro/macro_parser.h"
#include "../logs/logging_utils.h"

bool preCompile(char *arr[], char *am_file, int dex, bool *parse_failure) {
    Macros *macros;
    char temp_file1[PRE_MAX_LINE] = "";
    char temp_file2[PRE_MAX_LINE] = "";

    /* Insure file ends with am extension */
    if (!endsWithAS(arr[dex])) {
        logError("source file doesn't have a valid file extension\n");
        return false;
    }

    /* Removing unnecessary white spaces */
    if (!formatFile(arr[dex], temp_file1)) {
        return false;
    }

    /* For scanning and saving all the macros */
    macros = createMacros();
    if (macros == NULL) {
        return false;
    }

    /* processing Macros into macros and remove macros blocks where declared */
    if (!processMacroLines(macros, temp_file1, parse_failure) || !removeMacros(temp_file1, temp_file2, macros)) {
        freeMacros(macros);
        remove(temp_file1);
        return false;
    }
    /* no need for temp_file1 */
    remove(temp_file1);
    if (!replaceMacrosInFile(temp_file2, macros, am_file, *parse_failure)) {
        freeMacros(macros);
        remove(temp_file2);
        /* if file created */
        if (!am_file[0])
            remove(am_file);
        return false;
    }

    if (*parse_failure)
        strcpy(am_file, temp_file2);
    else
        remove(temp_file2);

    freeMacros(macros);
    return true;
}
