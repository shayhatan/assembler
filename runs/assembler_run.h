#ifndef ASSEMBLER_ASSEMBLER_RUN_H
#define ASSEMBLER_ASSEMBLER_RUN_H

/**
 * @brief Runs the assembler on the provided files.
 *
 * This function compiles the source files into object code and generates output files.
 *
 * @param files An array of strings containing the paths to the source files.
 * @param index The index of the current source file in the `files` array.
 */
void assemblerRun(char *files[], int index);

#endif /* ASSEMBLER_ASSEMBLER_RUN_H */
