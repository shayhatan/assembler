/*
 Created by User on 01/04/2024.
*/

#ifndef ASSEMBLER_STRING_UTILS_H
#define ASSEMBLER_STRING_UTILS_H

#include <stdbool.h>

/* duplicate str without allocation */
void duplicateStr(const char *original, char *target, int length);

/* duplicate str with allocation */
char *allocatedDuplicateString(char *s1);

int indexOfChar(const char *str, char ch);

void getStringBetweenSpaces(char *word, char *buffer);

bool isAlphaNumeric(char *word);

void strcpy_n(const char *src, char *target, int len);

int indexOfChar(const char *str, char ch);

bool isNumber(char *word);
bool tryGetNumber(char *word, int * number);

bool isQuotedString(char *word);
void removeExcessSpaces(char *input);
void resetString(char* string);

#endif /* ASSEMBLER_STRING_UTILS_H */
