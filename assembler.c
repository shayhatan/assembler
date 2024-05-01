#include "./runs/assembler_run.h"
#include "./logs/logging_utils.h"

/*
   ============= Hello Ester! 😊 ===========
   🌟 VERY IMPORTANT MESSAGE 🌟

   Before proceeding, please take a moment to review the assumptions outlined in the README file.
   This document contains critical information regarding the functionality and underlying assumptions of the program.

   Additionally, we've included a PDF file demonstrating error cases and the completion process using the "make" command in the terminal.

   Many of these assumptions were gleaned from insightful discussions on the course forum, which were contributed by you! 🌟
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
