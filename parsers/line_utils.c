/*
 Created by User on 09/04/2024.
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "./parse_types.h"
#include "./parsers_utils.h"
#include "./line_utils.h"
#include "../logs/logging_utils.h"
#include "../string_utils.h"

enum ParseResult parseLine(char *line, int lineNumber, input_line *result) {
    char temp_buffer[81];

    String temp_string;
    enum ParseResult status = PARSE_SUCCESS;

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
            logError("a label must be followed by at least one space character\n");
        }
        skipWhitespaces(&line);
    }

    temp_string = readNextString(&line, ' ', temp_buffer);

    if (strcmp(temp_buffer, "") == 0) {
        /* either an empty line or an empty label */
        return result->hasLabel ? PARSE_FAILURE : PARSE_SUCCESS;
    }

    /* instruction line */
    if (tryGetOpcode(temp_buffer, &result->opcode)) {
        if (tryGetArguments(line, STRING_TYPE, ANY, &result->arguments) != 0) {
            logError("invalid string arguments %s\n", line);
        }
        return PARSE_SUCCESS;
    }

    /* directive or constant line */
    if (tryGetDirectiveProps(temp_buffer, &result->directive_props)) {
        /* todo handle failure status regarding out of memory */
        switch (result->directive_props) {
            case dot_data:
                if (tryGetArguments(line, NUMERIC_TYPE, PLURAL, &result->arguments) != 0) {
                    logError("invalid numeric arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_string:
                if (tryGetArguments(line, DOUBLE_QUOTE_STRING, SINGLE, &result->arguments) != 0) {
                    logError("invalid quoted string argument %s\n", line);
                }
                break;
            case dot_external:
                if (tryGetArguments(line, LABEL_TYPE, PLURAL, &result->arguments) != 0) {
                    logError("invalid label arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_entry:
                if (tryGetArguments(line, LABEL_TYPE, SINGLE, &result->arguments) !=
                    0) {
                    logError("invalid label argument %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case dot_define:
                /* .define must not be under a label scope */
                if (result->hasLabel) {
                    logError("didn't expect constant definition after a label %s\n", line);
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

    logError(
            "invalid line definition, expected the next word to be an operation or directive but got \"%s\" instead\n",
            temp_buffer);
    return PARSE_FAILURE;
}

void resetLine(input_line *line) {
    line->label = NULL;
    line->hasLabel = false;
    line->directive_props = 0;
    line->const_definition_arg.constant_id = NULL;
    line->arguments.args_count = 0;
    line->opcode = -1;
    line->const_definition_arg.constant_value = 0;
    line->isComment = false;
    line->isEmpty = false;
    line->isEOF = false;
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
}
