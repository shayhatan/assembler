#include "./runs/assembler_run.h"
#include "./logs/logging_utils.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        logError("Expected at least one input file.\n");
        return 1000; /* invalid amount of args */
    }
    while (--argc > 0)
        assemblerRun(argv, argc);

    return 0;
}
