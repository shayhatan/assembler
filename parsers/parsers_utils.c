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
#include "../string_utils.h"

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


bool isOpcode(char *word) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(word, OPERATIONS[i]) == 0) {
            return true;
        }
    }
    return false;
}


bool isDirective(char *word) {
    int i;
    for (i = 0; i < 5; i++) {
        if (strcmp(word, DIRECTIVE_PROPS[i]) == 0) {
            return true;
        }
    }
    return false;
}

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


bool isValidVariableString(char *word) {
    return !isRegisterAddressing(word) && !isOpcode(word) && isAlphaNumeric(word) && !isDirective(word);
}


bool tryGetVariableString(char *word, char *buffer) {
    if (!isValidVariableString(word)) return false;
    getStringBetweenSpaces(word, buffer);
    return true;
}

bool isDirectAddressing(Operand operand) {
    return isValidVariableString(operand);
}

/* internal functions */
bool isLabelOrConstantString(char *word) {
    return isValidVariableString(word);
}

bool getTrue(char *word) {
    return true;
}

/* external functions */

/* mutates line */
/* returns a dynamically allocated sub-string copy of n chars */
String readNextString(char **line, char delimiter, char result_buffer[81]) {
    String result;
    result.content = result_buffer;
    result.size = 0;


    if (line == NULL || *line == NULL || **line == EOF) {
        return result; /* empty string means no more words */
    }

    result.size = indexOfChar(*line, delimiter);
    if (result.size != INT_MAX) {
        duplicateStr(*line, result_buffer, result.size);
        *line += result.size + 1;
        return result;
    }

    result.size = min(indexOfChar(*line, '\n'), strlen(*line));
    duplicateStr(*line, result_buffer, result.size);

    *line = NULL;

    return result;
}

void readTillNewLine(char **line, char buffer[81]) {
    if (line == NULL || *line == NULL || **line == EOF) {
        buffer[0] = '\0';
        return;
    }

    duplicateStr(*line, buffer, indexOfChar(*line, '\n'));
    *line = NULL;
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

char *getNthArgument(Arguments *args_container, int index) {
    return args_container->args[index];
}

enum ParseResult tryGetOperationWordsCounter(input_line *line, int *words_counter) {
    enum Addressing operandsAddressing[2] = {-1};

    int amountOfOperands = getAmountOfOperandsByOperation(line->opcode);

    if (line->arguments.args_count != amountOfOperands) {
        log_error("Arguments count %d does not match expected amount of operands %d", amountOfOperands);
        return PARSE_FAILURE;
    }

    switch (amountOfOperands) {
        case 0:
            *words_counter = 1;
            return PARSE_SUCCESS;
        case 1:
            operandsAddressing[0] = getAddressingForOperand(getNthArgument(&line->arguments, 1));
            /* single operand operations always involve a target operand */
            if (!isAddressingValid(operandsAddressing[0], line->opcode, target)) {
                return PARSE_FAILURE;
            }
            *words_counter = 1;
            return PARSE_SUCCESS;
        case 2:
            operandsAddressing[0] = getAddressingForOperand(getNthArgument(&line->arguments, 1));
            operandsAddressing[1] = getAddressingForOperand(getNthArgument(&line->arguments, 2));
            if (!isAddressingValid(operandsAddressing[0], line->opcode, source) ||
                !isAddressingValid(operandsAddressing[1], line->opcode, target)) {
                return PARSE_FAILURE;
            }
            /* both addressings are valid, check if they are different */
            /* yes --> we need 2 words, otherwise 1 word */
            *words_counter = operandsAddressing[0] != operandsAddressing[1] ? 2 : 1;
            return PARSE_SUCCESS;
        default:
            return PARSE_FAILURE; /* unreachable */
    }
}

/* todo fix this function */
bool isEOF(char *ptr) {
    return *ptr == EOF;
//    while (ptr != NULL) {
//        if (*ptr == EOF) {
//            return true;
//        }
//        ptr++;
//    }
//    return false;
}

int tryGetOpcode(char *word, enum opcode *result) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(word, OPERATIONS[i]) == 0) {
            *result = i;
            return true;
        }
    }
    return false; /* either -1 which means nothing detected, or a valid opcode */
}


/* collision (overlaps) are handled within first_run */
int tryGetDirectiveProps(char *word, enum DirectiveProps *result) {
    int i;
    for (i = 0; i < 5; i++) {
        if (strcmp(word, DIRECTIVE_PROPS[i]) == 0) {
            *result = (int) pow(2, i);
            return true;
        }
    }
    return false;
}

enum ParseResult tryGetLabelValue(char *line, char **result) {
    String temp;
    char buffer[81];
    temp.content = buffer;

    if (!doesContainLabel(line, &temp)) {
        return PARSE_FAILURE;
    }

    duplicateStr(line, buffer, temp.size);

    if (!isLabelOrConstantString(buffer)) {
        log_error("invalid label syntax %s", line);
        return PARSE_FAILURE;
    }

    *result = allocatedDuplicateString(buffer);
    if (result == NULL) {
        return OUT_OF_MEMORY;
    }
    return PARSE_SUCCESS;
}


bool doesContainLabel(char *line, String *result) {
    int brace_index;
    int col_index = indexOfChar(line, ':');
    /* : must appear outside of a string or char value and in the beginning of the sentence */
    brace_index = min(indexOfChar(line, '\''), indexOfChar(line, '"'));

    result->content = line;
    result->size = col_index;

    return col_index > 0 && col_index < brace_index;
}


void addArgument(Arguments *args, char arg[MAX_ARG_CHARS], int arg_index, int arg_size) {
    duplicateStr(arg, (*args).args[arg_index], arg_size);
}

enum ParseResult _tryGetArguments(char *line, enum ArgumentsCountType expectedAmount, Arguments args,
                                  ValidateArgumentFunction validator) {
    String next_string;
    char temp[81];

    if (line == NULL) {
        return PARSE_FAILURE; /* line is out of buffer */
    }

    while (line != NULL) {
        next_string = readNextString(&line, ',', temp);

        if (next_string.size == 0) {
            break;
        }

        if (validator(temp)) {
            addArgument(&args, next_string.content, args.args_count, next_string.size);
        }

        args.args_count++;
    }

    if (expectedAmount == SINGLE && args.args_count > 1) {
        log_error("too many args detected");
        return PARSE_FAILURE;
    }

    if (expectedAmount == PLURAL) {
        if (args.args_count > 0) {
            return PARSE_SUCCESS;
        }
        log_error("missing args detected");
        return PARSE_FAILURE;
    }
    return PARSE_SUCCESS;
}

enum ParseResult
tryGetArguments(char *line, enum ArgumentType type, enum ArgumentsCountType expectedAmount, Arguments *args) {
    switch (type) {
        case NUMERIC_TYPE:
            return _tryGetArguments(line, expectedAmount, *args, isNumber) != 0;
        case DOUBLE_QUOTE_STRING:
            return _tryGetArguments(line, expectedAmount, *args, isQuotedString) != 0;
        case LABEL_TYPE:
            return _tryGetArguments(line, expectedAmount, *args, isLabelOrConstantString) != 0;
        case STRING_TYPE:
            return _tryGetArguments(line, expectedAmount, *args, getTrue) != 0;
    }
}

int tryGetAssignmentArgument(char *line, DefinitionArgument *argument) {
    char temp_buffer[81];
    char *ptr = temp_buffer;
    String temp_string;
    char *endptr;
    /* todo check if line is still valid */
    if (line == NULL) {
        log_error("missing constant definition %s", line);
        return PARSE_FAILURE; /* line is out of buffer */
    }
    temp_string = readNextString(&line, '=', temp_buffer);

    skipWhitespaces(&ptr);

    if (temp_string.size == 0) {
        log_error("invalid constant id %s\n", line);
        return PARSE_FAILURE;
    }
    if (!tryGetVariableString(temp_string.content, ptr)) {
        log_error("invalid constant id syntax %s\n", line);
        return PARSE_FAILURE;
    }
    argument->constant_id = allocatedDuplicateString(ptr);
    if (argument->constant_id == NULL) {
        return OUT_OF_MEMORY;
    }

    /* todo: remove possible lines in the suffix and prefix of constant_id, raise error if there is a space between two words of it for exammple "SSS fff=" (invalid) "    SSSfff   " (valid) */
    readTillNewLine(&line, temp_buffer);

    if (temp_buffer[0] == '\0') {
        log_error("missing constant value ");
        return PARSE_FAILURE;
    }

    argument->constant_value = strtol(temp_buffer, &endptr, 10);
    if (*endptr != '\0') {
        /*...input is not a decimal number */
        log_error("invalid numeric constant value %s\n", line);
        return 1;
    }

    return 0;
}

void skipWhitespaces(char **line) {
    if (line == NULL || *line == NULL) return;
    while (isspace(**line)) {
        (*line)++;
    }
}