//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "./parsers/parse_types.h"
#include "labels_table.h"
#include "./logs/logging_utils.h"
#include "factory.h"
#include "./parsers/parsers_utils.h"
#include "first_run.h"
#include "string_utils.h"

bool errored;
unsigned int IC = 0, DC = 0;

enum ParseResult parseLine(char *line, int lineNumber, input_line *result) {
    char temp_buffer[81];
    String temp_string;
    enum ParseResult status;

    result->lineNumber = lineNumber;
    result->isEOF = isEOF(line);

    /* stop only if this line is the EOF and its first char is the actual EOF */
    if (result->isEOF && *line == EOF) {
        return PARSE_SUCCESS;
    }

    /* empty line */
    if (strcmp("\n", line) == 0) {
        result->isEmpty = true;
        return PARSE_SUCCESS;
    }

    /* ignore comments */
    if (*line == ';') {
        result->isComment = true;
        return PARSE_SUCCESS;
    }

    skipWhitespaces(&line);


    if (doesContainLabel(line, &temp_string)) {
        status = tryGetLabelValue(line, &result->label);
        if (status > PARSE_SUCCESS) {
            return status;
        }

        result->hasLabel = true;
        /* skip to the instruction following the label*/
        /* todo: after label empty line */
        line += strlen(result->label) + 1;

        if (!isspace(*line)) {
            log_error("after a label at least one space must appear\n");
        }
        skipWhitespaces(&line);
    }

    temp_string = readNextString(&line, ' ', temp_buffer);

    if (strcmp(temp_buffer, "") == 0) {
        /* either an empty line or an empty label */
        return PARSE_SUCCESS;
    }

    /* instruction line */
    if (tryGetOpcode(temp_buffer, &result->opcode)) {
        if (tryGetArguments(line, STRING_TYPE, ANY, &result->arguments) != 0) {
            log_error("invalid string arguments %s\n", line);
        }
        return PARSE_SUCCESS;
    }

    /* directive or constant line */
    if (tryGetDirectiveProps(temp_buffer, &result->directive_props)) {
        /* todo handle failure status regarding out of memory */
        switch (result->directive_props) {
            case dot_data:
                if (tryGetArguments(line, NUMERIC_TYPE, PLURAL, &result->arguments) != 0) {
                    log_error("invalid numeric arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_string:
                if (tryGetArguments(line, DOUBLE_QUOTE_STRING, SINGLE, &result->arguments) != 0) {
                    log_error("invalid quoted string argument %s\n", line);
                }
                break;
            case dot_external:
                if (tryGetArguments(line, LABEL_TYPE, PLURAL, &result->arguments) != 0) {
                    log_error("invalid label arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_entry:
                if (tryGetArguments(line, LABEL_TYPE, SINGLE, &result->arguments) !=
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
        return status;
    }

    log_error(
            "invalid line definition %s, expected the next word to be an operation or directive but got %s instead\n",
            line, temp_buffer);
    return PARSE_FAILURE;
};

void resetLine(input_line *line) {
    line->label = NULL;
    line->hasLabel = false;
    line->directive_props = 0;
    line->const_definition_arg.constant_id = NULL;
    line->arguments.args_count = 0;
    line->const_definition_arg.constant_value = 0;
    line->isComment = false;
    line->isEmpty = false;
    line->isEOF = false;
    line->hasLabel = false;
}

void disposeLine(input_line *line) {
    if (line->hasLabel) {
        free(line->label);
        line->label = NULL;
        line->hasLabel = false;
    }

    if (line->const_definition_arg.constant_id != NULL) {
        free(line->const_definition_arg.constant_id);
        line->const_definition_arg.constant_id = NULL;
    }

    line->arguments.args_count = 0;

    line->const_definition_arg.constant_value = 0;
    line->isComment = false;
    line->isEOF = false;
    line->hasLabel = false;
};


void countStringWords(char *label, char *strPtr) {
    while (*strPtr != '\0') {
        /* each character is one word */
        incrementLabelWordsCounter(label);
        strPtr++;
    }
    /* also increment for '\0' */
    incrementLabelWordsCounter(label);
}

int countDataWords(char *label, void *ptr) {
    entry *temp;
    char *strPtr = ptr;
    if (isNumber(strPtr)) {
        incrementLabelWordsCounter(label);
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
    int L;
    if (line.isEOF) {
        return STOP;
    }

    /* step 4 */
    if (line.directive_props & dot_define) {
        setLabel(line.const_definition_arg.constant_id,
                 createEntry(DOT_DEFINE, line.const_definition_arg.constant_value), true);
        return NEXT;
    }

    /* steps 5, 6, 7 */
    if (line.directive_props & (dot_data | dot_string) && line.hasLabel) {
        entry *addedEntry;
        /* step 8 */
        if (setLabel(line.label, createEntry(DOT_DATA, (int) DC), true) != 0) {
            return NEXT;
        }
        /* step 9 (.string case) */
        /* increase DC according to arguments */
        if (line.directive_props & dot_string) {
            /* in-case of a .string the list has 1 node which contains a pointer to the entire string */
            countStringWords(line.label, line.arguments.args[0]);
        }
        /* step 9 (.data case) */
        if (line.directive_props & dot_data) {
            int index;
            incrementLabelWordsCounter(line.label);
            addedEntry = get_data(line.label);
            for (index = 0; index < line.arguments.args_count; index++) {
                if (!countDataWords(line.label, line.arguments.args[index])) {
                    DC += addedEntry->wordsCounter;
                    return NEXT;
                }
            }
        }

        /* it's impossible for addedEntry to be null in this case */
        addedEntry = get_data(line.label);
        DC += addedEntry->wordsCounter;
        return NEXT;
    }

    /* step 10, 11 */
    if (line.directive_props & dot_external) {
        bulkAddExternalOperands(&line.arguments, true);
        return NEXT;
    }

    /* step 10 (.entry) */
    if ((line.directive_props & dot_entry)) {
        return NEXT;
    }

    /* step 12 */
    if (line.hasLabel && setLabel(line.label, createEntry(DOT_CODE, ((int) IC) + 100), true) != 0) {
        return NEXT;
    }

    /* step 13 */
    if (line.opcode < 0 || line.opcode > 15) {
        log_error("invalid operation %d", line.opcode);
        return NEXT;
    }

    /* steps 14, 15 */
    if (tryGetOperationWordsCounter(&line, &L) != PARSE_SUCCESS) {
        log_error("failed to get operand words");
    }
    IC += L;

    return NEXT;
}

enum ParseResult run(FILE *srcFile) {
    char buffer[81];
    int index = 0;

    labelsTableInit();

    while (fgets(buffer, 81, srcFile) != 0) {
        input_line line;
        enum ParseResult parse_result;
        bool shouldStop = false;

        resetLine(&line);

        parse_result = parseLine(buffer, index++, &line);

        switch (parse_result) {
            case PARSE_FAILURE:
                log_error("Failed to parse line %d %s\n", index - 1, buffer);
                disposeLine(&line);
                continue;
            case OUT_OF_MEMORY:
                log_error("gracefully clearing all allocations and shutting down\n");
                disposeLine(&line);
                labelsTableDispose();
                return OUT_OF_MEMORY; /* complete bail out */
        }


        if (line.isComment || line.isEmpty) {
            disposeLine(&line);
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
        return PARSE_FAILURE; /* custom error code */
    }

    /*update all symbols with data classification to IC + 100 */
    updateDataLabels(IC);
    return 0;
}