#include <stdlib.h>
#include "./assembler_utils.h"
#include "../utils/memory.h"

MapResult assemblerInit(Assembler *assembler) {
    assembler->tables = NULL;
    assembler->IC = 0;
    assembler->DC = 0;
    assembler->has_dot_ent = false;
    assembler->tables = allocateMemory(sizeof(Tables));

    if (assembler->tables == NULL) {
        return MAP_OUT_OF_MEMORY;
    }


    /* Initialize Tables */
    if (tablesInit(assembler->tables) == MAP_OUT_OF_MEMORY) {
        free(assembler->tables);
        return MAP_OUT_OF_MEMORY;
    }


    return MAP_SUCCESS;
}

void assemblerDispose(Assembler *assembler) {
    printf("cleanup initiated\n");
    if (assembler->tables != NULL) {
        /*Dispose Tables*/
        tablesDispose(assembler->tables);
        free(assembler->tables); /*Free memory allocated for Tables*/
        assembler->tables = NULL; /*Reset pointer to NULL*/
    }
}
