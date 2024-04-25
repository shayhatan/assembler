
#ifndef MMN14_MACRO_PARSER_H
#define MMN14_MACRO_PARSER_H

#include <string.h>
#include <assert.h>
#include "macros.h"
#include <stdio.h>

/**
 * Function to read macro content from file.
 * @param fp The file pointer to the input file.
 * @param pos The position in the file to start reading from.
 * @param line_count Pointer to store the number of lines read.
 * @return A pointer to the macro content if successful, NULL otherwise.
 */
char *readMacroData(FILE *fp, fpos_t *pos, int *line_count);

/**
 * Function to process a line of code when adding a macro definition.
 * @param line The line of code to process.
 * @param line_number The line number of the input file.
 * @param name Pointer to store the name of the macro if processed successfully.
 * @param error Pointer to store error flag.
 * @return true if processing is successful, false otherwise.
 */
bool processAddMcrLine(char *line, int line_number, char* name, bool* error);

/**
 * Function to check if a token is the last Word in a line.
 * @param token The token to check.
 * @return true if the token is the last Word in the line, false otherwise.
 */
bool isLastLineWord(const char *token);

/**
 * Function to process macro lines in a formatted file.
 * @param macros Pointer to the Macros structure.
 * @param formatted_file_name The name of the formatted file to process.
 * @return true if processing is successful, false otherwise.
 */
bool processMacroLines(Macros *macros, const char *formatted_file_name);

/**
 * Function to replace occurrences of macro names with allocated data in a file.
 * @param filename The name of the input file.
 * @param macros Pointer to the Macros structure containing macro definitions.
 * @param am_file Pointer to store the name of the output file with replaced macros.
 * @return true if replacement is successful, false otherwise.
 */
bool replaceMacrosInFile(const char *filename, Macros *macros, char* am_file);

/**
 * Function to remove macro blocks from a source file and save the result to a destination file.
 * @param source_filename The name of the source file.
 * @param destination_filename Pointer to store the name of the destination file.
 * @param macros Pointer to the Macros structure containing macro definitions.
 * @return true if removal is successful, false otherwise.
 */
bool removeMacros(const char *source_filename, char *destination_filename, Macros *macros);
#endif /* MMN14_MACRO_PARSER_H */
