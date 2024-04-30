#include "./runs/assembler_run.h"
#include "./logs/logging_utils.h"

/*
   Must read the README file and assumptions for proper understanding.
   It contains crucial information regarding the program's functionality and assumptions.
   Most of the assumptions were derived from the course forum discussions.
*/

int main(int argc, char *argv[]) {
    if (argc == 1) {
        logError("Expected at least one input file.\n");
        return 1000; /* invalid amount of args */
    }
    while (--argc > 0)
        assemblerRun(argv, argc);

    return 0;
}
