//
// Created by User on 20/03/2024.
//


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <minmax.h>
#include "parsers/parse_types.h"
#include "./logs/logging_utils.h"
#include "./parsers_utils.h"
#include "../factory.h"

const int TWO_OPERANDS_OPERATIONS[] = {mov, cmp, add, sub, lea};
const int ONE_OPERAND_OPERATIONS[] = {not, clr, inc, dec, jmp, bne, red, prn, jsr};
const int ZERO_OPERANDS_OPERATIONS[] = {rts, stop};

/* todo move to consts*/
char *OPERATIONS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                      "rts", "stop"};

int OPERATIONS_LENGTHS[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4};
/* todo move to consts*/
char *DIRECTIVE_PROPS[] = {".data", ".string", ".external", ".entry", ".define"};
int DIRECTIVES_LENGTHS[] = {5, 6, 9, 6, 7};


bool isRegisterAddressing(Operand operand) {
    return ((operand[0] == 'r' && '0' <= operand[1] && operand[1] <= '7' && operand[2] == '\0') ? true : false);
}

bool isIndirectRegisterAddressing(Operand operand) {
    return (operand[0] == '*') && isRegisterAddressing(operand + 1);
}

bool isInstantAddressing(Operand operand) {
    char *endPtr;
    if (operand[0] != '#') return false;

    strtol(operand, &endPtr, 10);
    return endPtr == operand || *endPtr != '\0';
}

bool isDirectAddressing(Operand operand) {
    char *current = operand;

    /* must not be a register */
    if (isRegisterAddressing(operand)) {
        return false;
    }

    /* must begin with a letter */
    if (!isalpha(*current)) {
        return false;
    }
    while (current != NULL) {
        /* only alphanumeric letters are allowed */
        if (!isalnum(*current)) {
            return false;
        }
        current++;
    }
    return true;
}

/* internal functions */
static int _indexOfChar(const char *str, char ch) {
    char *ptr = strchr(str, ch);
    if (ptr != NULL) {
        return ptr - str; // Calculate the index by subtracting the pointers
    } else {
        return -1; // Character not found
    }
}


/* returns a dynamically allocated sub-string copy of n chars */
static char *_strdupn(const char *str, size_t n) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    if (n > len) {
        n = len;  // If n is greater than the length of the string, limit it to the string length
    }

    char *copy = (char *) malloc((n + 1) * sizeof(char)); // Allocate memory for the copy (+1 for null terminator)
    if (copy == NULL) {
        return NULL; // Memory allocation failed
    }

    strncpy(copy, str, n); // Copy at most n characters
    copy[n] = '\0'; // Null-terminate the copied string

    return copy;
}


bool isNumber(char *word) {
    char *endptr;
    strtol(word, &endptr, 10);
    return *endptr != '\0';
}

bool isQuotedString(char *word) {
    if (word == NULL) return false;
    if (*word != '\"') return false;
    if (*(word + strlen(word) - 1) != '\"') return false;

    return true;
}

bool isOpcode(char *word) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(word, OPERATIONS[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool isLabelOrConstantString(char *word) {
    char *iterator = word;

    if (word == NULL) return false;

    if (isRegisterAddressing(word)) return false;

    while (iterator != NULL) {
        if (!islower(*iterator)) return false;
        iterator++;
    }

    if (isOpcode(word)) {
        return false;
    }

    return true;
}

bool getTrue(char *word) {
    return true;
}

/* external functions */

/* mutates line */
/* returns a dynamically allocated sub-string copy of n chars */
char *readNextString(char **line, char delimiter) {
    char *result;

    if (line == NULL || *line == NULL || **line == EOF) {
        return strdup(""); /* empty string means no more words */
    }

    int index_of_space = _indexOfChar(*line, delimiter);
    if (index_of_space > 0) {
        *line += index_of_space;
        return _strdupn(*line, index_of_space);
    }

    result = _strdupn(*line, _indexOfChar(*line, '\0'));
    *line = NULL;

    return result;
}

/* returns a dynamically allocated sub-string copy of n chars */
char *getNextString(char **line, char delimiter) {
    char *result;

    if (line == NULL || *line == NULL) {
        return NULL;
    }

    int index_of_space = _indexOfChar(*line, delimiter);
    if (index_of_space > 0) {
        return _strdupn(*line, index_of_space);
    }

    result = _strdupn(*line, _indexOfChar(*line, '\0'));

    return result;
}


/* returns an enum flag */
int getAllowedSourceOperandAddressingsByOpcode(enum opcode op) {
    if (op == lea) {
        return direct;
    }
    if (op == mov || op == cmp || op == add || op == sub) {
        return instant |
               direct |
               indirectRegister |
               directRegister; /* all addressings are valid */
    }
    return 0; /* no source operand thus no addressings */
}

/* returns an enum flag */
int getAllowedTargetOperandAddressingsByOpcode(enum opcode op) {
    if (op == cmp || op == prn) {
        return instant |
               direct |
               indirectRegister |
               directRegister; /* all addressings are valid */
    }
    if (op == jsr) {
        return direct |
               indirectRegister;
    }
    if (op == rts || op == stop) {
        return 0; /* no target operand thus no addressings */
    }
    return direct |
           indirectRegister |
           directRegister;
}

int getAmountOfOperandsByOperation(enum opcode code) {
    if (code < 5) {
        return 2;
    }
    if (code < 14) {
        return 1;
    }
    return 0;
}


enum Addressing getAddressingForOperand(Operand operand) {
    if (isInstantAddressing(operand)) return instant;
    if (isDirectAddressing(operand)) return direct;
    if (isIndirectRegisterAddressing(operand)) return indirectRegister;
    if (isRegisterAddressing(operand)) return directRegister;

    return -1;
}

enum Destination {
    source, target
};

bool isAddressingValid(enum Addressing operandsAddressing, enum opcode opcode, enum Destination destination) {
    if (operandsAddressing == -1) {
        log_error("Invalid addressing, operands%s, doesn't have a defined addressing", operandsAddressing);
        return false;
    }

    /* is the translated addressing forbidden for the operand of this operation? */
    switch (destination) {
        case source:
            if (!(operandsAddressing & getAllowedSourceOperandAddressingsByOpcode(opcode))) {
                log_error("Invalid addressing, operand %s, is not allowed for opcode %d as source addressing",
                          operandsAddressing, opcode);
                return false;
            }
            break;
        case target:
            if (!(operandsAddressing & getAllowedTargetOperandAddressingsByOpcode(opcode))) {
                log_error("Invalid addressing, operand %s, is not allowed for opcode %d as target addressing",
                          operandsAddressing, opcode);
                return false;
            }
    }
    return true;
}

int getOperationWordsCounter(input_line *line) {
    enum Addressing operandsAddressing[2] = {-1};
    int argsCount;
    int amountOfOperands = getAmountOfOperandsByOperation(line->opcode);

    argsCount = listLength(line->arguments->strings);
    if (argsCount != amountOfOperands) {
        log_error("Arguments count %d does not match expected amount of operands %d", amountOfOperands);
        return -1;
    }

    switch (amountOfOperands) {
        case 0:
            return 0;
        case 1:
            operandsAddressing[0] = getAddressingForOperand(getNth(line->arguments->strings, 1)->value);
            /* single operand operations always involve a target operand */
            if (!isAddressingValid(operandsAddressing[0], line->opcode, target)) {
                return -1;
            }
            return 1;
        case 2:
            operandsAddressing[0] = getAddressingForOperand(getNth(line->arguments->strings, 1)->value);
            operandsAddressing[1] = getAddressingForOperand(getNth(line->arguments->strings, 2)->value);
            if (!isAddressingValid(operandsAddressing[0], line->opcode, source) ||
                !isAddressingValid(operandsAddressing[1], line->opcode, target)) {
                return -1;
            }
            /* both addressings are valid, check if they are different */
            /* yes --> we need 2 words, otherwise 1 word */
            return operandsAddressing[0] != operandsAddressing[1] ? 2 : 1;
        default:
            return -2; /* unreachable */
    }
}

bool isEOF(char *ptr) {
    while (ptr != NULL) {
        if (*ptr == EOF) {
            return true;
        }
        ptr++;
    }
    return false;
}

enum opcode getOpcode(char *line) {
    int i;
    for (i = 0; i < 16; i++) {
        char *operation_ptr = strstr(line, OPERATIONS[i]);
        if (operation_ptr != NULL) {
            return i;
        }
    }
    return -1; /* either -1 which means nothing detected, or a valid opcode */
}


/* collision (overlaps) are handled within first_run */
enum DirectiveProps getDirectiveProps(char *line) {
    int i;
    for (i = 0; i < 16; i++) {
        char *operation_ptr = strstr(line, OPERATIONS[i]);
        if (operation_ptr != NULL) {
            return (int) pow(2, i);
        }
    }
    return 0;
}

char *getLabelValue(char *line) {
    int wordEnd;
    if (!doesContainLabel(line)) {
        return NULL;
    }
    wordEnd = _indexOfChar(line, ':');
    if (wordEnd > 0) {
        return _strdupn(line, wordEnd); /* todo: mal-allocation */
    }
    return NULL;
}


bool doesContainLabel(char *line) {
    int brace_index;
    int col_index = _indexOfChar(line, ':');
    /* : must appear outside of a string or char value and in the beginning of the sentence */
    brace_index = min(_indexOfChar(line, '\''), _indexOfChar(line, '"'));

    return col_index > 0 && col_index < brace_index;
}

int _tryGetArguments(char *line, enum ArgumentType type, enum ArgumentsCountType expectedAmount, List args,
                     ValidateArgumentFunction validator) {
    int current_args_amount = 0;
    /* todo check if line is still valid */
    if (line == NULL) {
        return 2; /* line is out of buffer */
    }
    switch (type) {
        case NUMERIC_TYPE:
            args = createIntegerList();
            break;
        case DOUBLE_QUOTE_STRING:
        case STRING_TYPE:
        case LABEL_TYPE:
            args = createStringList();
            break;
    }

    while (line != NULL) {
        int next_int;
        char *next_string = readNextString(&line, ',');
        if (next_string == NULL) {
            break;
        }
        /* maybe we should stop here ?? */
        if (current_args_amount > expectedAmount) {
            log_error("too many args detected");
            return 3;
        }
        if (type == NUMERIC_TYPE) {
            char *endptr;
            if (!validator(next_string)) {
                return 1;
            }
            next_int = strtol(next_string, &endptr, 10);
            if (*endptr != '\0') {
                /*...input is not a decimal number */
                return 1;
            } else {
                addLast(args, &next_int);
            }
        } else {
            if (!validator(next_string)) {
                return 1;
            }
            addLast(args, next_string);
        }
        free(next_string);
        current_args_amount++;
    }

    return 0;
}

int tryGetArguments(char *line, enum ArgumentType type, enum ArgumentsCountType expectedAmount, List args) {
    switch (type) {
        case NUMERIC_TYPE:
            return _tryGetArguments(line, type, expectedAmount, args, isNumber) != 0;
        case DOUBLE_QUOTE_STRING:
            return _tryGetArguments(line, type, expectedAmount, args, isQuotedString) != 0;
        case LABEL_TYPE:
            return _tryGetArguments(line, type, expectedAmount, args, isLabelOrConstantString) != 0;
        case STRING_TYPE:
            return _tryGetArguments(line, type, expectedAmount, args, getTrue) != 0;
    }
}

int tryGetAssignmentArgument(char *line, DefinitionArgument argument) {
    char *endptr;
    /* todo check if line is still valid */
    if (line == NULL) {
        return 2; /* line is out of buffer */
    }
    argument->constant_id = readNextString(&line, '=');

    if (argument->constant_id == NULL) {
        log_error("invalid constant id %s\n", line);
        return 2; /* todo invalid constant name or allocation */
    }

    /* skip to the constant value string */
    line += strlen(argument->constant_id) + 1;

    argument->constant_value = strtol(line, &endptr, 10);
    if (*endptr != '\0') {
        /*...input is not a decimal number */
        log_error("invalid numeric constant value %s\n", line);
        return 1;
    }


    return 0;
}