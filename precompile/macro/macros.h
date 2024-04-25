#ifndef MACROS_H
#define MACROS_H

#include <stdbool.h>
#include "../../utils/file_manager.h"
#include "../../utils/memory.h"
#include "../precompile_list/sorted_list.h"
#include <string.h>

#define PRE_MAX_LINE 81

/* Struct for storing macro information */
typedef struct {
    char* macro_name;
    char *allocated_data;
} Macro;

/* Struct for storing macros with associated sorted list */
typedef struct {
    SortedList *macros;
} Macros;

/**
 * Function to create a Macros structure.
 * @return A pointer to the newly created Macros structure.
 */
Macros *createMacros();

/**
 * Function to free memory allocated for a Macros structure.
 * @param macros Pointer to the Macros structure to free.
 */
void freeMacros(Macros *macros);

/**
 * Function to insert a macro into the Macros structure.
 * @param macros Pointer to the Macros structure.
 * @param macro_name The name of the macro to insert.
 * @param data The data associated with the macro.
 */
void insertMacro(Macros *macros, const char *macro_name, char *data);

/**
 * Function to get the data associated with a macro.
 * @param macros Pointer to the Macros structure.
 * @param macro_name The name of the macro.
 * @return The data associated with the macro, or NULL if not found.
 */
char *getMacroData(Macros *macros, const char *macro_name);

/**
 * Function to check if the Macros structure contains a specific macro.
 * @param macros Pointer to the Macros structure.
 * @param macro_name The name of the macro to check.
 * @return true if the macro is found, false otherwise.
 */
bool isContainMacro(Macros *macros, const char *macro_name);

/**
 * Function to create a Macro structure.
 * @param macro_name The name of the macro.
 * @param data The data associated with the macro.
 * @return A pointer to the newly created Macro structure.
 */
Macro *createMacro(char *macro_name, char *data);

/**
 * Function to replace macro occurrences in a file and write the result to a new file.
 * @param file_name The name of the input file.
 * @param out_put The name of the output file.
 * @param macros Pointer to the Macros structure containing macro definitions.
 */
void replaceMacros(const char *file_name, char *temp_file_name, Macros *macros);

#endif /* MACROS_H */

