#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "./parse_types.h"
#include "../logs/logging_utils.h"
#include "./parsers_utils.h"
#include "../utils/string_utils.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

/* todo move to consts*/
char *OPERATIONS[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                      "rts", "hlt"};

/* todo move to consts*/
char *DIRECTIVE_PROPS[] = {".data", ".string", ".extern", ".Entry", ".define"};


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
    if (operand[0] != '#') return false;

    return isNumber(operand+1) || isValidVariableString(operand+1);
}


bool isValidVariableString(char *word) {
    return !isRegisterAddressing(word) && !isOpcode(word) && isAlphaNumeric(word) && !isDirective(word);
}

bool isConstantIndexString(Operand operand) {
    char arrName[DEF_MAX_ARG_CHARS], indexStr[DEF_MAX_ARG_CHARS];
    if (operand == NULL || *operand == '\0' || !isalpha(*operand)) return false;
    if (sscanf(operand, "%[a-zA-Z0-9_] [%[^]]]", arrName, indexStr) != 2) {
        return false;
    }
    return isAlphaNumeric(arrName) && (isAlphaNumeric(indexStr) || isNumber(indexStr));
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
        return result; /* empty string means no more words_map */
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

    duplicateStr(*line, buffer, min(indexOfChar(*line, '\n'), strlen(*line)));
    *line = NULL;
}

/* returns an enum flag */
int getAllowedSourceOperandAddressingsByOpcode(enum Opcode op) {
    if (op == lea) {
        return DIRECT | CONSTANT_INDEX;
    }
    if (op == mov || op == cmp || op == add || op == sub) {
        return INSTANT |
               DIRECT |
               DIRECT_REGISTER |
               CONSTANT_INDEX; /* all addressings are valid */
    }
    return 0; /* no source operand thus no addressings */
}

/* returns an enum flag */
int getAllowedTargetOperandAddressingsByOpcode(enum Opcode op) {
    if (op == cmp || op == prn) {
        return INSTANT |
               DIRECT |
               DIRECT_REGISTER |
               CONSTANT_INDEX; /* all addressings are valid */
    }
    if (op == jsr || op == bne || op == jmp) {
        return DIRECT |
               DIRECT_REGISTER;
    }
    if (op == rts || op == hlt) {
        return 0; /* no target operand thus no addressings */
    }
    return DIRECT |
           CONSTANT_INDEX |
           DIRECT_REGISTER;
}

int getAmountOfOperandsByOperation(enum Opcode code) {
    if (code < 4 || code == lea) {
        return 2;
    }
    if (code < 14) {
        return 1;
    }
    return 0;
}


enum Addressing getAddressingFlagForOperand(Operand operand) {
    if (isInstantAddressing(operand)) return INSTANT;
    if (isDirectAddressing(operand)) return DIRECT;
    if (isRegisterAddressing(operand)) return DIRECT_REGISTER;
    if (isConstantIndexString(operand)) return CONSTANT_INDEX;

    return -1;
}

int getAddressingForOperand(Operand operand) {
    if (isInstantAddressing(operand)) return 0;
    if (isDirectAddressing(operand)) return 1;
    if (isConstantIndexString(operand)) return 2;
    if (isRegisterAddressing(operand)) return 3;

    return -1;
}

enum Destination {
    source, target
};

bool isAddressingValid(enum Addressing operandsAddressing, enum Opcode opcode, enum Destination destination) {
    if (operandsAddressing == -1) {
        logError("Invalid addressing\n");
        return false;
    }

    /* is the translated addressing forbidden for the operand of this operation? */
    switch (destination) {
        case source:
            if (!(operandsAddressing & getAllowedSourceOperandAddressingsByOpcode(opcode))) {
                logError("Invalid addressing, operand %d, is not allowed for Opcode %d as source addressing\n",
                         operandsAddressing, opcode);
                return false;
            }
            break;
        case target:
            if (!(operandsAddressing & getAllowedTargetOperandAddressingsByOpcode(opcode))) {
                logError("Invalid addressing, operand %s, is not allowed for Opcode %d as target addressing",
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
        logError("Arguments count %d does not match expected amount of operands %d\n", line->arguments.args_count,
                 amountOfOperands);
        return PARSE_FAILURE;
    }

    switch (amountOfOperands) {
        case 0:
            *words_counter = 1;
            return PARSE_SUCCESS;
        case 1:
            operandsAddressing[0] = getAddressingFlagForOperand(getNthArgument(&line->arguments, 0));
            /* single operand operations always involve a target operand */
            if (!isAddressingValid(operandsAddressing[0], line->opcode, target)) {
                return PARSE_FAILURE;
            }
            *words_counter = 1;
            /* constant index addressing adds 2 extra words_map */
            *words_counter += operandsAddressing[0] == CONSTANT_INDEX ? 2 : 1;
            return PARSE_SUCCESS;
        case 2:
            operandsAddressing[0] = getAddressingFlagForOperand(getNthArgument(&line->arguments, 0));
            operandsAddressing[1] = getAddressingFlagForOperand(getNthArgument(&line->arguments, 1));
            if (!isAddressingValid(operandsAddressing[0], line->opcode, source) ||
                !isAddressingValid(operandsAddressing[1], line->opcode, target)) {
                return PARSE_FAILURE;
            }
            *words_counter = 1;
            if (operandsAddressing[0] == CONSTANT_INDEX) {
                *words_counter += 2;
                /* in case both are constant index we can't ever unify the extra words_map unlike other addressings */
                *words_counter += operandsAddressing[1] == CONSTANT_INDEX ? 2 : 1;
                return PARSE_SUCCESS;
            }
            /* src is not a constant index but target is -- we need 3 words_map to represent both */
            if (operandsAddressing[1] == CONSTANT_INDEX) {
                *words_counter += 3;
                return PARSE_SUCCESS;
            }
            /* neither addressings are constant index unify if possible otherwise use 2 words_map */
            *words_counter += operandsAddressing[0] == operandsAddressing[1] ? 1 : 2;
            return PARSE_SUCCESS;
        default:
            return PARSE_FAILURE; /* unreachable */
    }
}

bool isEOF(char *ptr) {
    return *ptr == EOF;
}

int tryGetOpcode(char *word, enum Opcode *result) {
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(word, OPERATIONS[i]) == 0) {
            *result = i;
            return true;
        }
    }
    return false; /* either -1 which means nothing detected, or a valid Opcode */
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
        logError("invalid label syntax %s", line);
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


void addArgument(Arguments *args, char arg[DEF_MAX_ARG_CHARS], int arg_index, int arg_size) {
    duplicateStr(arg, (*args).args[arg_index], arg_size);
}

typedef void ((ExtractArgumentFunction)(char *, char *));

ParseResult _tryGetArguments(char *line, ArgumentsCountType expectedAmount, Arguments *args,
                                  ValidateArgumentFunction validator, ExtractArgumentFunction extractor) {
    String next_string;
    char temp[81];

    if (line == NULL) {
        return expectedAmount == ANY ? PARSE_SUCCESS : PARSE_FAILURE; /* line is out of buffer */
    }

    while (line != NULL) {
        next_string = readNextString(&line, ',', temp);

        if (next_string.size == 0) {
            break;
        }

        if (validator(temp)) {
            extractor(next_string.content, next_string.content);
            addArgument(args, next_string.content, args->args_count, next_string.size);
        } else {
            logError("argument %s doesn't match the expected format\n", temp);
            return PARSE_FAILURE;
        }

        args->args_count++;
    }

    if (expectedAmount == SINGLE && args->args_count > 1) {
        logError("too many args detected");
        return PARSE_FAILURE;
    }

    if (expectedAmount == PLURAL) {
        if (args->args_count > 0) {
            return PARSE_SUCCESS;
        }
        logError("missing args detected");
        return PARSE_FAILURE;
    }
    return PARSE_SUCCESS;
}

void extractQuote(char *src, char *target) {
    sscanf(src, "%*[\"]%[^\"\n]%*[\"]", target);
}

bool isIntegerOrVariableName(char *word) { return isNumber(word) || isValidVariableString(word); }

ParseResult tryGetArguments(char *line, ArgumentType type, ArgumentsCountType expected_amount, Arguments *args) {
    switch (type) {
        case NUMERIC_TYPE:
            return _tryGetArguments(line, expected_amount, args, isIntegerOrVariableName, getStringBetweenSpaces) != 0;
        case DOUBLE_QUOTE_STRING:
            return _tryGetArguments(line, expected_amount, args, isQuotedString, extractQuote) != 0;
        case LABEL_TYPE:
            return _tryGetArguments(line, expected_amount, args, isLabelOrConstantString, getStringBetweenSpaces) != 0;
        case STRING_TYPE:
            return _tryGetArguments(line, expected_amount, args, getTrue, getStringBetweenSpaces) != 0;
    }
    return PARSE_FAILURE;
}

int tryGetAssignmentArgument(char *line, DefinitionArgument *argument) {
    char temp_buffer[81];
    char *ptr = temp_buffer;
    String temp_string;
    if (line == NULL) {
        logError("missing constant definition %s", line);
        return PARSE_FAILURE; /* line is out of buffer */
    }
    temp_string = readNextString(&line, '=', temp_buffer);

    skipWhitespaces(&ptr);

    if (temp_string.size == 0) {
        logError("invalid constant id %s\n", line);
        return PARSE_FAILURE;
    }
    if (!tryGetVariableString(temp_string.content, ptr)) {
        if (line == NULL) {
            logError("invalid constant id syntax NULL\n");
        }
        else {
            logError("invalid constant id syntax %s\n", line);
        }
        return PARSE_FAILURE;
    }
    argument->constant_id = allocatedDuplicateString(ptr);
    if (argument->constant_id == NULL) {
        return OUT_OF_MEMORY;
    }

    readTillNewLine(&line, temp_buffer);

    if (temp_buffer[0] == '\0') {
        logError("missing constant value\n");
        return PARSE_FAILURE;
    }
    
    if (!tryGetNumber(temp_buffer, &argument->constant_value)) {
        /*...input is not a decimal number */
        if (line == NULL) {
            logError("invalid numeric constant value NULL\n");
        }
        else {
            logError("invalid numeric constant value %s\n", line);
        }
        return PARSE_FAILURE;
    }

    return PARSE_SUCCESS;
}

void skipWhitespaces(char **line) {
    if (line == NULL || *line == NULL) return;
    while (isspace(**line)) {
        (*line)++;
    }
}