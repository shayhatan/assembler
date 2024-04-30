#ifndef ASSEMBLER_ASSEMBLER_UTILS_H
#define ASSEMBLER_ASSEMBLER_UTILS_H

#include "../tables/tables.h"

typedef struct {
    Tables *tables;
    unsigned int IC;
    unsigned int DC;
    bool has_dot_ent;
} Assembler;

/**
 * @brief Initializes the assembler.
 *
 * This function initializes the assembler structure by allocating memory for its tables and setting initial values.
 *
 * @param assembler Pointer to the Assembler structure to initialize.
 * @return MapResult indicating success or failure.
 */
MapResult assemblerInit(Assembler *assembler);

/**
 * @brief Disposes resources allocated by the assembler.
 *
 * This function frees memory allocated for the assembler's tables and resets the assembler structure.
 *
 * @param assembler Pointer to the Assembler structure to dispose.
 */
void assemblerDispose(Assembler *assembler);

#endif /*ASSEMBLER_ASSEMBLER_UTILS_H*/
