//
// Created by User on 01/04/2024.
//

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <minmax.h>
#include "logs/logging_utils.h"

/* duplicate str without allocation */
void duplicateStr(const char *original, char *target, int length) {
    int i = 0;
    for (; i < length; i++) {
        target[i] = original[i];
    }
    target[i] = '\0';
}

char *allocatedDuplicateString(char *s1) {
    char *s2 = strdup(s1);
    if (s2 == NULL) {
        log_error("Error: Unable to allocate memory.\n");
    }
    return s2;
}

bool isAlphaNumeric(char *word) {
    bool trailingSpaces = false;
    if (word == NULL) return false;
    char *current = word;

    /* skip initial spaces */
    while (isspace(*current)) current++;

    /* must begin with a letter */
    if (!isalpha(*current)) {
        return false;
    }
    while (current != NULL && *current != '\0') {

        if (isspace(*current)) {
            trailingSpaces = true;
            current++;
            continue;
        }

        /* only alphanumeric letters are allowed */
        if (!isalnum(*current)) {
            return false;
        }
        /* letters after spaces */
        if (trailingSpaces) {
            return false;
        }
        current++;
    }
    return true;
}

int indexOfChar(const char *str, char ch) {
    char *ptr = strchr(str, ch);
    if (ptr != NULL) {
        return ptr - str; // Calculate the index by subtracting the pointers
    } else {
        return INT_MAX; // Character not found
    }
}


void getStringBetweenSpaces(char *word, char *buffer) {
    int i;
    int end_index;
    char *current = word;

    while (isspace(*current)) current++;

    /* find word end */
    end_index = min(indexOfChar(current, ' '), indexOfChar(current, '\0'));

    for (i = 0; i < end_index; i++) {
        buffer[i] = current[i];
    }
    buffer[i] = '\0';
}

void strcpy_n(const char *src, char *target, int len) {
    int i;
    for (i = 0; i < len; ++i) {
        target[i] = src[i];
    }
    target[i] = '\0';
}

bool isNumber(char *word) {
    char *current = word;
    bool hadTrailingSpaces = false;
    if (word == NULL) return false;

    while (isspace(*current)) current++;

    if (*current == '-')
        current++;

    while (*current != '\0') {
        if (isspace(*current)) {
            hadTrailingSpaces = true;
            current++;
            continue;
        }
        if (!isdigit(*current)) return false;
        if (hadTrailingSpaces) return false;
        current++;
    }

    return true;
}

bool isQuotedString(char *word) {
    if (word == NULL) return false;
    if (*word != '\"') return false;
    if (*(word + strlen(word) - 1) != '\"') return false;

    /* todo: validate no 3rd " in between the last and first */

    return true;
}
