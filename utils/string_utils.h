#ifndef ASSEMBLER_STRING_UTILS_H
#define ASSEMBLER_STRING_UTILS_H

#include <stdbool.h>


/**
 * Duplicates a string without allocating memory.
 *
 * @param original The original string to duplicate.
 * @param target The target buffer to store the duplicated string.
 * @param length The length of the original string.
 */
void duplicateStr(const char *original, char *target, int length);

/**
 * Duplicates a string with memory allocation.
 *
 * @param s1 The string to duplicate.
 * @return A pointer to the newly allocated duplicated string.
 */
char *allocatedDuplicateString(const char *s1);

/**
 * Finds the index of the first occurrence of a character in a string.
 *
 * @param str The string to search.
 * @param ch The character to find.
 * @return The index of the first occurrence of the character, or -1 if not found.
 */
int indexOfChar(const char *str, char ch);

/**
 * Gets the string between spaces from a given word.
 *
 * @param word The word containing the string.
 * @param buffer The buffer to store the extracted string.
 */
void getStringBetweenSpaces(char *word, char *buffer);

/**
 * Checks if a string is alphanumeric.
 *
 * @param word The string to check.
 * @return True if the string is alphanumeric, false otherwise.
 */
bool isAlphaNumeric(char *word);

/**
 * Checks if a string is a number.
 *
 * @param word The character to check.
 * @return True if the character is a number, false otherwise.
 */
bool isNumber(char *word);

/**
 * Tries to parse a number from a string.
 *
 * @param word The string to parse.
 * @param number Pointer to store the parsed number.
 * @return True if successful, false otherwise.
 */
bool tryGetNumber(char *word, int *number);

/**
 * Checks if a string is quoted.
 *
 * @param word The string to check.
 * @return True if the string is quoted, false otherwise.
 */
bool isQuotedString(char *word);

/**
 * Removes excess spaces from a given input string.
 *
 * @param input The input string to remove excess spaces from.
 */
void removeExcessSpaces(char *input);

/**
 * Resets a string by setting all characters to '\0'.
 *
 * @param string The string to reset.
 */
void resetString(char* string);

/**
 * Checks if a macro name is valid.
 *
 * @param macro_name The macro name to check.
 * @return True if the macro name is valid, false otherwise.
 */
bool isValidMacroName(char* macro_name);


#endif /* ASSEMBLER_STRING_UTILS_H */
