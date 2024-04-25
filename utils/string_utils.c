#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "../logs/logging_utils.h"
#include "./memory.h"
#define INSTRUCTION_SIZE 4
#define CMD_SIZE 15
#define REG_SIZE 8
#define min(a, b) (((a) < (b)) ? (a) : (b))


/* duplicate str without allocation */
void duplicateStr(const char *original, char *target, int length) {
    int i = 0;
    for (; i < length; i++) {
        target[i] = original[i];
    }
    target[i] = '\0';
}

char *allocatedDuplicateString(const char *str) {
    size_t len = strlen(str) + 1;
    char *copy = (char *) allocateMemory(len);
    if (str == NULL)
        return NULL;

    if (copy == NULL)
        return NULL;

    memcpy(copy, str, len);
    if (copy == NULL) {
        logError("Error: Unable to allocate memory.\n");
    }
    return copy;
}

bool isAlphaNumeric(char *word) {
    char *current = word;
    bool trailingSpaces = false;
    if (word == NULL) return false;

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
        return ptr - str; /* Calculate the index by subtracting the pointers */
    } else {
        return INT_MAX; /* Character not found */
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

bool tryGetNumber(char *word, int * number) {
    if (!isNumber(word)) return false;
    *number = atoi(word);
    return true;
}

bool isQuotedString(char *word) {
    if (word == NULL) return false;
    if (*word != '\"') return false;
    if (*(word + strlen(word) - 1) != '\"') return false;

    return true;
}

/**
 * @brief Removes excess spaces from a string.
 *
 * Removes multiple consecutive spaces and replaces them with a single space.
 *
 * @param input The input string to be modified.
 */
void removeExcessSpaces(char *input) {
    int i = 0, j = 0;
    bool space_flag = false;
    /* avoid use cases where it starts with space */
    while (isspace(input[i])) {
        ++i;
    }

    for (; input[i] != '\0'; i++) {

        if (isspace(input[i])) {
            space_flag = true;
        } else {
            if (space_flag && input[i] != ' ') {
                input[j++] = ' ';
                space_flag = false;

            }
            input[j++] = input[i];
        }
    }

    input[j] = '\n';
    input[j++] = '\0';
}

bool isInArray(char* str,  char *array[], int size) {
    int i;

    if (str == NULL || array == NULL) {
        return false; /* Return false if either the string or the array is NULL*/
    }

    for (i = 0; i < size; i++) {
        if (strcmp(str, array[i]) == 0) {
            return true; /* Return true if the string is found in the array */
        }
    }
    return false; /* Return false if the string is not found in the array */
}

void resetString(char* string) {
    int i = 0;
    int length = strlen(string);

    for (; i < length; i++) {
        string[i] = '\0';
    }
}

bool isInstruction(char *str) {
    char *inst[] = {".data", ".string", ".extern", ".entry"};
    return isInArray(str, inst, INSTRUCTION_SIZE);
}

bool isCmd(char *str) {
    char *cmd[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea",
                   "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts",
                   "hlt"};

    return isInArray(str, cmd, CMD_SIZE);

}

bool isRegister(char* str) {
    char* regs[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    return isInArray(str, regs, REG_SIZE);
}

bool isValidMacroName(char* macro_name) {
    return !(isRegister(macro_name) || isCmd(macro_name) || isInstruction(macro_name));
}
