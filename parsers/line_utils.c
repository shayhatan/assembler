#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "./parse_types.h"
#include "./parsers_utils.h"
#include "./line_utils.h"
#include "../logs/logging_utils.h"

enum ParseResult parseLine(char *line, int line_number, InputLine *result) {
    char temp_buffer[81];

    String temp_string;
    enum ParseResult status = PARSE_SUCCESS;

    result->line_number = line_number;
    result->isEOF = isEOF(line);

    /* stop only if this line is the EOF and its first char is the actual EOF */
    if (result->isEOF && *line == EOF) {
        return PARSE_SUCCESS;
    }

    /* empty line */
    if (strcmp("\n", line) == 0) {
        result->is_empty = true;
        return PARSE_SUCCESS;
    }

    /* ignore comments */
    if (*line == ';') {
        result->is_comment = true;
        return PARSE_SUCCESS;
    }

    skipWhitespaces(&line);


    if (doesContainLabel(line, &temp_string)) {
        status = tryGetLabelValue(line, &result->label);
        if (status > PARSE_SUCCESS) {
            return status;
        }

        result->has_label = true;
        /* skip to the instruction following the label*/
        line += strlen(result->label) + 1;

        if (!isspace(*line)) {
            logError("a label must be followed by at least one space character\n");
        }
        skipWhitespaces(&line);
    }

    temp_string = readNextString(&line, ' ', temp_buffer);

    if (strcmp(temp_buffer, "") == 0) {
        /* either an empty line or an empty label */
        return result->has_label ? PARSE_FAILURE : PARSE_SUCCESS;
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
        switch (result->directive_props) {
            case DOT_DATA:
                if (tryGetArguments(line, NUMERIC_TYPE, PLURAL, &result->arguments) != 0) {
                    logError("invalid numeric arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case DOT_STRING:
                if (tryGetArguments(line, DOUBLE_QUOTE_STRING, SINGLE, &result->arguments) != 0) {
                    logError("invalid quoted string argument %s\n", line);
                }
                break;
            case DOT_EXTERNAL:
                if (tryGetArguments(line, LABEL_TYPE, PLURAL, &result->arguments) != 0) {
                    logError("invalid label arguments %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case DOT_ENTRY:
                if (tryGetArguments(line, LABEL_TYPE, SINGLE, &result->arguments) !=
                    0) {
                    logError("invalid label argument %s\n", line);
                    status = PARSE_FAILURE;
                }
                break;
            case DOT_DEFINE:
                /* .define must not be under a label scope */
                if (result->has_label) {
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
            "invalid line definition, expected the next Word to be an operation or directive but got \"%s\" instead\n",
            temp_buffer);
    return PARSE_FAILURE;
}

void resetLine(InputLine *line) {
    line->label = NULL;
    line->has_label = false;
    line->directive_props = 0;
    line->const_definition_arg.constant_id = NULL;
    line->arguments.args_count = 0;
    line->opcode = -1;
    line->const_definition_arg.constant_value = 0;
    line->is_comment = false;
    line->is_empty = false;
    line->isEOF = false;
}

void disposeLine(InputLine *line) {
    if (line->has_label) {
        free(line->label);
        line->label = NULL;
        line->has_label = false;
    }

    if (line->const_definition_arg.constant_id != NULL) {
        free(line->const_definition_arg.constant_id);
        line->const_definition_arg.constant_id = NULL;
    }

    line->arguments.args_count = 0;

    line->const_definition_arg.constant_value = 0;
    line->is_comment = false;
    line->is_empty = false;
    line->isEOF = false;
}
