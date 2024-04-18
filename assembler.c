
#include "runs/assembler_run.h"


int main(int argc, char *argv[]) {
    while (--argc > 0)
        assemblerRun(argv, argc);
    return 1;
}
