
#include "runs/assembler_run.h"


int main(int argc, char *argv[]) {
    char am_file[81] = "";
    while (--argc > 0)
        assemblerRun(argv, argc);
    return 1;
}
