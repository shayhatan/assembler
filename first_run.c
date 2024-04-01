//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "./parsers/parse_types.h"
#include "labels_table.h"
#include "./logs/logging_utils.h"
#include "factory.h"
#include "./parsers/parsers_utils.h"

bool errored;
unsigned int IC = 0, DC = 0;

enum ParseResult {
    PARSE_FAILURE, SUCCESS
};


enum ParseResult parseLine(char *line, int lineNumber, input_line *result) {
    char *temp;
    List temp_args;

    result->lineNumber = lineNumber;
    result->isEOF = isEOF(line);

    /* stop only if this line is the EOF and its first char is the actual EOF */
    if (result->isEOF && *line == EOF) {
        return SUCCESS;
    }

    /* empty line */
    if (strcmp("\n", line) == 0) {
        result->isEmpty = true;
        return SUCCESS;
    }

    /* ignore comments */
    if (*line == ';') {
        result->isComment = true;
        return SUCCESS;
    }

    skipWhitespaces(&line);

    result->hasLabel = doesContainLabel(line);
    if (result->hasLabel) {
        result->label = getLabelValue(line);
        /* skip to the instruction following the label*/
        /* todo: after label empty line */
        line += strlen(result->label) + 1;
        /* todo: validate that following the label exists a whitespace*/
        skipWhitespaces(&line);
    }

    temp = readNextString(&line, ' ');
    if (temp == NULL) {
        /* Out of memory */
        log_error("out of memory");
        /* todo: allocation error */
        return PARSE_FAILURE;
    }
    if (strcmp(temp, "") == 0) {
        /* either an empty line or an empty label */
        return SUCCESS;
    }

    /* instruction line */
    if (tryGetOpcode(temp, &result->opcode)) {
        if (tryGetArguments(line, STRING_TYPE, PLURAL, result->arguments) != 0) {
            log_error("invalid string arguments %s\n", line);
        }
        free(temp);
        return SUCCESS;
    }

    /* directive or constant line */
    if (tryGetDirectiveProps(temp, &result->directive_props)) {
        enum ParseResult status = SUCCESS;
        /* todo handle failure status regarding out of memory */
        switch (result->directive_props) {
            case dot_data:
                if (tryGetArguments(line, NUMERIC_TYPE, PLURAL, result->arguments) != 0) {
                    log_error("invalid numeric arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_string:
                if (tryGetArguments(line, DOUBLE_QUOTE_STRING, SINGLE, result->arguments) != 0) {
                    log_error("invalid quoted string argument %s\n", line);
                }
                break;
            case dot_external:
                if (tryGetArguments(line, LABEL_TYPE, PLURAL, result->arguments) !=
                    0) {
                    log_error("invalid label arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_entry:
                if (tryGetArguments(line, LABEL_TYPE, SINGLE, result->arguments) !=
                    0) {
                    log_error("invalid label argument %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_define:
                /* .define must not be under a label scope */
                if (result->hasLabel) {
                    log_error("didn't expect constant definition after a label %s\n", line);
                    status = PARSE_FAILURE;
                    break;
                }
                if (tryGetAssignmentArgument(line, &result->const_definition_arg) != 0) {
                    status = PARSE_FAILURE;
                }
                break;
        }
        free(temp);
        return status;
    }

    log_error(
            "invalid line definition %s, expected the next word to be an operation or directive but got %s instead",
            line, temp);
    free(temp);
    return PARSE_FAILURE;
};

void disposeLine(input_line *line) {

    if (line->arguments != NULL) {
        listDispose(line->arguments);
    }

    if (line->hasLabel)
        free(line->label);

    line->const_definition_arg.constant_id = NULL;
    line->const_definition_arg.constant_value = 0;
    line->isComment = false;
    line->isEOF = false;
    line->hasLabel = false;
};


enum analyze_status {
    STOP, NEXT
};

static input_line currentLine;

void countStringWords(char *strPtr) {
    while (strPtr != NULL) {
        /* each character is one word */
        incrementLabelWordsCounter(currentLine.label);
        strPtr++;
    }
}

int countDataWords(unsigned int _, void *ptr) {
    entry *temp;
    char *strPtr = ptr;
    if (isNumber(strPtr)) {
        incrementLabelWordsCounter(currentLine.label);
        return true;
    }
    temp = get_data(strPtr); /* in case we passed a symbol */
    if (temp == NULL) {
        log_error("non-existent symbol %s cannot be a .data argument", strPtr);
        return false;
    }
    if (strcmp(temp->classification, DOT_DEFINE) != 0) { /* in case the symbol is not a constant */
        log_error(".data symbol %s must be a constant definition", strPtr);
        return false;
    }
    incrementLabelWordsCounter(strPtr);
    return true;
}

enum analyze_status analyze_line(input_line line) {
    if (line.isEOF) {
        return STOP;
    }

    /* step 4 */
    if (line.directive_props & dot_define) {
        addLabel(line.const_definition_arg.constant_id,
                 createEntry(DOT_DEFINE, line.const_definition_arg.constant_value), true);
        return NEXT;
    }

    /* steps 5, 6, 7 */
    if (line.directive_props & (dot_data | dot_string) && line.hasLabel) {
        entry *addedEntry;
        /* step 8 */
        if (addLabel(line.label, createEntry(DOT_DATA, (int) DC), true) != 0) {
            return NEXT;
        }
        /* step 9 (.string case) */
        /* increase DC according to arguments */
        if (line.directive_props & dot_string) {
            /* in-case of a .string the list has 1 node which contains a pointer to the entire string */
            countStringWords(getFirst(line.arguments)->value);
        }
        /* step 9 (.data case) */
        if (line.directive_props & dot_data) {
            incrementLabelWordsCounter(line.label);
            addedEntry = get_data(line.label);
            if (!iterate(line.arguments, countDataWords)) {
                DC += addedEntry->wordsCounter;
                return NEXT;
            }
        }

        /* it's impossible for addedEntry to be null in this case */
        addedEntry = get_data(line.label);
        DC += addedEntry->wordsCounter;
        return NEXT;
    }

    /* step 10, 11 */
    if (line.directive_props & dot_external) {
        bulkAddExternalOperands(line.arguments, false);
        return NEXT;
    }

    /* step 10 (.entry) */
    if ((line.directive_props & dot_entry)) {
        return NEXT;
    }

    /* step 12 */
    if (line.hasLabel && !addLabel(line.label, createEntry(DOT_CODE, ((int) IC) + 100), true)) {
        return NEXT;
    }

    /* step 13 */
    if (line.opcode < 0 || line.opcode > 15) {
        log_error("invalid operation %d", line.opcode);
        return NEXT;
    }

    /* steps 14, 15 */
    IC += /* L */ getOperationWordsCounter(&line);

    return NEXT;
}

int run(FILE *srcFile) {
    char buffer[81];

    int index = 0;
    while (fgets(buffer, 81, srcFile)) {
        input_line line;
        line.label = NULL;

        enum ParseResult parse_result;
        bool shouldStop = false;
        parse_result = parseLine(buffer, index++, &line);

        if (parse_result == PARSE_FAILURE) {
            log_error("Failed to parse line %d %s\n", index - 1, buffer);
            continue;
        }
        if (line.isComment || line.isEmpty) {
            continue;
        }

        switch (analyze_line(line)) {
            case NEXT:
                break;
            case STOP:
                shouldStop = true;
                break;
        }
        disposeLine(&line);
        if (shouldStop) {
            break;
        }
    }

    if (errored) {
        return 1000; /* custom error code */
    }

    /*update all symbols with data classification to IC + 100 */
    updateDataLabels(IC);
    return 0;
}