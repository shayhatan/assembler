
#include "assembler.h"
#include <stdlib.h>

void assemblerInit(Assembler *assembler) {
    assembler->tables = malloc(sizeof(Tables)); /* @Itay change func*/
    if (assembler->tables == NULL) {
        /* @itay Handle memory allocation error */
        fprintf(stderr, "Error: Memory allocation failed for Tables\n");
    }

    /* Initialize Tables */
    tablesInit(assembler->tables);

    /* Initialize IC and DC */
    assembler->IC = 0;
    assembler->DC = 0;
    assembler->has_dot_ent = false;
}

void assemblerDispose(Assembler *assembler) {
    if (assembler->tables != NULL) {
        // Dispose Tables
        tablesDispose(assembler->tables);
        free(assembler->tables); // Free memory allocated for Tables
        assembler->tables = NULL; // Reset pointer to NULL
    }
}
