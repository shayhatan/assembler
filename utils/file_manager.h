#ifndef MMN14_FORMAT_PARSER_H
#define MMN14_FORMAT_PARSER_H

#include <stdbool.h>
#include <stdio.h>
#define MAX_FIlE_LINE 81

/**
 * @file format_parser.h
 * @brief Header file for string formatting functions.
 */

/**
 * @brief Formats a string by removing excess spaces and compacting commas.
 *
 * Removes excess spaces and multiple consecutive commas from the input string,
 * ensuring that only one space or one comma separates words.
 *
 * @param input The input string to be formatted.
 */
void formatString(char *input);

/**
 * @brief Formats a file by removing excess spaces and compacting commas from each line.
 *
 * Reads each line from the input file, formats it, and writes the formatted lines to the output file.
 *
 * @param input_file_name The name of the input file to be formatted.
 * @param new_file1 The name of the output file.
 * @return true if formatting is successful, false otherwise.
 */
bool formatFile(const char *input_file_name, char *new_file1);

/**
 * @brief Generates the output file name by adding a specified extension.
 *
 * @param input_file_name The name of the input file.
 * @param output_file_name The name of the output file to be generated.
 * @param extension The extension to be added to the output file name.
 */
void generateOutputFileName(const char *input_file_name, char *output_file_name, const char* extension);

/**
 * @brief Checks if a line exceeds the maximum allowed length.
 *
 * @param line The line to be checked.
 * @return true if the line exceeds the maximum length, false otherwise.
 */
bool isLineTooLong(const char *line);

/**
 * @brief Copies the content of a text file into a dynamically allocated memory buffer.
 *
 * @param fp The file pointer to the input text file.
 * @param pos The position indicator in the file.
 * @param length The length of the text to be copied.
 * @return A pointer to the dynamically allocated memory buffer containing the file content.
 * if allocation failed returns null
 */
char *copyTextFile(FILE *fp, fpos_t *pos, int length);

/**
 * Check if a file name ends with ".am".
 *
 * @param file_name The name of the file to check.
 * @return true if the file name ends with ".am", false otherwise.
 */
bool endsWithAS(const char *file_name);

#endif /* MMN14_FORMAT_PARSER_H */
