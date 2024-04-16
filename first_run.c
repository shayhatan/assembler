/*
 Created by User on 17/03/2024.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "./parsers/parse_types.h"
#include "./labels_table.h"
#include "./logs/logging_utils.h"
#include "./factory.h"
#include "./parsers/parsers_utils.h"
#include "./first_run.h"
#include "./string_utils.h"
#include "./parsers/line_utils.h"
#include "./decode_table.h"
#include "./words/decoders.h"
#include <ctype.h>


bool errored;
unsigned int IC = 0, DC = 0;

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
    temp = getEntry(strPtr); /* in case we passed a symbol */
    if (temp == NULL) {
        logError("non-existent symbol %s cannot be a .data argument", strPtr);
        return false;
    }
    if (strcmp(temp->classification, DOT_DEFINE) != 0) { /* in case the symbol is not a constant */
        logError(".data symbol %s must be a constant definition", strPtr);
        return false;
    }
    incrementLabelWordsCounter(strPtr);
    return true;
}

static AnalyzeStatus analyzeLine(input_line line) {
    int L;
    MapResult status = MAP_SUCCESS;

    if (line.isEOF) {
        return STOP;
    }

    /* step 4 */
    if (line.directive_props & dot_define) {
        if (setLabel(line.const_definition_arg.constant_id,
                     createEntry(DOT_DEFINE, line.const_definition_arg.constant_value), true) == MAP_OUT_OF_MEMORY) {
            return ANALYZE_OUT_OF_MEMORY;
        }
        return NEXT;
    }

    /* steps 5, 6, 7 */
    if (line.directive_props & (dot_data | dot_string) && line.hasLabel) {
        entry *addedEntry;
        /* step 8 */
        status = setLabel(line.label, createEntry(DOT_DATA, (int) DC), true);
        if (status != MAP_SUCCESS) {
            return status == MAP_OUT_OF_MEMORY ? ANALYZE_OUT_OF_MEMORY : NEXT;
        }
        /* step 9 (.string case) */
        /* increase DC according to arguments */
        if (line.directive_props & dot_string) {
            int temp;
            addedEntry = getEntry(line.label);
            temp = addedEntry->value;

            /* in-case of a .string the list has 1 node which contains a pointer to the entire string */
            countStringWords(line.label, line.arguments.args[0]);
            status = decodeString(&temp, &line.arguments);
            if (status != MAP_SUCCESS) {
                return status == MAP_OUT_OF_MEMORY ? ANALYZE_OUT_OF_MEMORY : NEXT;
            }
        }
        /* step 9 (.data case) */
        if (line.directive_props & dot_data) {
            int index;
            int temp;

            incrementLabelWordsCounter(line.label);
            addedEntry = getEntry(line.label);
            temp = addedEntry->value;

            for (index = 0; index < line.arguments.args_count; index++) {

                if (!countDataWords(line.label, line.arguments.args[index])) {
                    DC += addedEntry->wordsCounter;
                    return NEXT;
                }
            }
            status = decodeData(&temp, &line.arguments);
            if (status != MAP_SUCCESS) {
                return status == MAP_OUT_OF_MEMORY ? ANALYZE_OUT_OF_MEMORY : NEXT;
            }
        }

        /* it's impossible for addedEntry to be null in this case */
        addedEntry = getEntry(line.label);
        DC += addedEntry->wordsCounter;
        return NEXT;
    }

    /* step 10, 11 */
    if (line.directive_props & dot_external) {
        status = bulkAddExternalOperands(&line.arguments, true);
        return status != MAP_OUT_OF_MEMORY ? NEXT : ANALYZE_OUT_OF_MEMORY;
    }

    /* step 10 (.entry) */
    if ((line.directive_props & dot_entry)) {
        return NEXT;
    }

    /* step 12 */
    if (line.hasLabel) {
        status = setLabel(line.label, createEntry(DOT_CODE, ((int) IC) + 100), true);
        if (status != MAP_SUCCESS) {
            return status == MAP_OUT_OF_MEMORY ? ANALYZE_OUT_OF_MEMORY : NEXT;
        }
    }

    /* step 13 */
    if (line.opcode < 0 || line.opcode > 15) {
        logError("invalid operation %d\n", (int) line.opcode);
        return NEXT;
    }

    /* steps 14, 15 */
    if (tryGetOperationWordsCounter(&line, &L) != PARSE_SUCCESS) {
        logError("failed to get operand words_map\n");
    }
    IC += L;

    return NEXT;
}

ParseResult run(FILE *srcFile) {
    char buffer[81] = "";
    int index = 0;

    while (fgets(buffer, 81, srcFile) != 0) {
        input_line line;
        ParseResult parse_result;
        bool shouldStop = false;
        removeExcessSpaces(buffer);
        resetLine(&line);

        setLogLineContext(index, buffer);

        parse_result = parseLine(buffer, index++, &line);

        switch (parse_result) {
            case PARSE_FAILURE:
                logError("Failed to parse line %d %s\n", index - 1, buffer);
                disposeLine(&line);
                continue;
            case OUT_OF_MEMORY:
                logError("gracefully clearing all allocations and shutting down\n");
                disposeLine(&line);
                labelsTableDispose();
                return OUT_OF_MEMORY; /* complete bail out */
            case PARSE_SUCCESS: /* do nothing */
                break;
        }


        if (line.isComment || line.isEmpty) {
            disposeLine(&line);
            continue;
        }

        switch (analyzeLine(line)) {
            case NEXT:
                break;
            case STOP:
                shouldStop = true;
                break;
            case ANALYZE_OUT_OF_MEMORY:
                shouldStop = true;
                logError("Out of memory!\n");
                break;
        }
        disposeLine(&line);
        memset(buffer, 0, sizeof(buffer)); /* clear buffer */
        if (shouldStop) {
            break;
        }
    }

    if (errored) {
        return PARSE_FAILURE; /* custom error code */
    }

    /*update all symbols with data classification to IC + 100 */
    updateDataLabels(IC);
    wordUpdateDecode((int)IC + 100);
    return PARSE_SUCCESS;
}