
#include "precomplie/precompile.h"
#include "precomplie/precompile_consts.h"


void runBookletExample(char *argv[]) {
    char am_file[PRE_MAX_LINE] = "";
    preCompile(argv, am_file, 1);
}


int main(int argc, char *argv[]) {
    runBookletExample(argv);
    return 0;
}

