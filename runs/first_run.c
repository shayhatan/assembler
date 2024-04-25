#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../parsers/parse_types.h"
#include "../tables/labels_table.h"
#include "../logs/logging_utils.h"
#include "../utils/factory.h"
#include "../parsers/parsers_utils.h"
#include "./first_run.h"
#include "../utils/string_utils.h"
#include "../parsers/line_utils.h"
#include "../words/decoders.h"


void countStringWords(char *label, char *strPtr, Assembler *assembler) {
    while (*strPtr != '\0') {
        /* each character is one Word */
        incrementLabelWordsCounter(label, assembler->tables->labels_table);
        strPtr++;
    }
    /* also increment for '\0' */
    incrementLabelWordsCounter(label, assembler->tables->labels_table);
}

int countDataWords(char *label, void *ptr, Assembler *assembler) {
    Entry *temp;
    char *strPtr = ptr;
    if (isNumber(strPtr)) {
        incrementLabelWordsCounter(label, assembler->tables->labels_table);
        return true;
    }
    temp = getEntry(strPtr, assembler->tables->labels_table); /* in case we passed a symbol */
    if (temp == NULL) {
        logError("non-existent symbol %s cannot be a .data argument", strPtr);
        return false;
    }
    if (strcmp(temp->classification, DEF_DOT_DEFINE) != 0) { /* in case the symbol is not a constant */
        logError(".data symbol %s must be a constant definition", strPtr);
        return false;
    }
    incrementLabelWordsCounter(label, assembler->tables->labels_table);
    return true;
}

static AnalyzeStatus analyzeLine(input_line line, Assembler *assembler) {
    int L;
    MapResult status = MAP_SUCCESS;

    if (line.isEOF) {
        return STOP;
    }

    /* step 4 */
    if (line.directive_props & DOT_DEFINE) {
        if (setLabel(line.const_definition_arg.constant_id,
                     createEntry(DEF_DOT_DEFINE, line.const_definition_arg.constant_value), true,
                     assembler->tables->labels_table) == MAP_OUT_OF_MEMORY) {
            return ANALYZE_OUT_OF_MEMORY;
        }
        return NEXT;
    }

    /* steps 5, 6, 7 */
    if (line.directive_props & (DOT_DATA | DOT_STRING) && line.has_label) {
        Entry *addedEntry;
        /* step 8 */
        status = setLabel(line.label, createEntry(DEF_DOT_DATA, (int) assembler->DC), true, assembler->tables->labels_table);
        if (status != MAP_SUCCESS) {
            return status == MAP_OUT_OF_MEMORY ? ANALYZE_OUT_OF_MEMORY : NEXT;
        }
        /* step 9 (.string case) */
        /* increase DC according to arguments */
        if (line.directive_props & DOT_STRING) {
            int temp;
            addedEntry = getEntry(line.label, assembler->tables->labels_table);
            temp = addedEntry->value;

            /* in-case of a .string the list has 1 node which contains a pointer to the entire string */
            countStringWords(line.label, line.arguments.args[0], assembler);
            status = decodeString(&temp, &line.arguments, assembler->tables->words_map);
            if (status != MAP_SUCCESS) {
                return status == MAP_OUT_OF_MEMORY ? ANALYZE_OUT_OF_MEMORY : NEXT;
            }
        }
        /* step 9 (.data case) */
        if (line.directive_props & DOT_DATA) {
            int index;
            int temp;

            addedEntry = getEntry(line.label, assembler->tables->labels_table);
            temp = addedEntry->value;

            for (index = 0; index < line.arguments.args_count; index++) {

                if (!countDataWords(line.label, line.arguments.args[index], assembler)) {
                    assembler->DC += addedEntry->words_counter;
                    return NEXT;
                }
            }
            status = decodeData(&temp, &line.arguments, assembler->tables->labels_table, assembler->tables->words_map);
            if (status != MAP_SUCCESS) {
                return status == MAP_OUT_OF_MEMORY ? ANALYZE_OUT_OF_MEMORY : NEXT;
            }
        }

        /* it's impossible for addedEntry to be null in this case */
        addedEntry = getEntry(line.label, assembler->tables->labels_table);
        assembler->DC += addedEntry->words_counter;
        return NEXT;
    }

    /* step 10, 11 */
    if (line.directive_props & DOT_EXTERNAL) {
        status = bulkAddExternalOperands(&line.arguments, true, assembler->tables->labels_table);
        return status != MAP_OUT_OF_MEMORY ? NEXT : ANALYZE_OUT_OF_MEMORY;
    }

    /* step 10 (.Entry) */
    if ((line.directive_props & DOT_ENTRY)) {
        return NEXT;
    }

    /* step 12 */
    if (line.has_label) {
        status = setLabel(line.label, createEntry(DEF_DOT_CODE, ((int) assembler->IC) + 100), true,
                          assembler->tables->labels_table);
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
    assembler->IC += L;

    return NEXT;
}

ParseResult run(FILE *src_file, Assembler *assembler) {
    char buffer[81] = "";
    int index = 1;
    bool errored = false;

    while (fgets(buffer, 81, src_file) != 0) {
        input_line line;
        ParseResult parse_result;
        bool shouldStop = false;
        resetLine(&line);

        setLogLineContext(index, buffer, "first-run");

        parse_result = parseLine(buffer, index++, &line);

        if (!errored) {
            errored = parse_result != PARSE_SUCCESS;
        }

        switch (parse_result) {
            case PARSE_FAILURE:
                logError("Failed to parse line %d %s\n", index - 1, buffer);
                disposeLine(&line);
                continue;
            case OUT_OF_MEMORY:
                logError("gracefully clearing all allocations and shutting down\n");
                disposeLine(&line);
                return OUT_OF_MEMORY; /* complete bail out */
            case PARSE_SUCCESS: /* do nothing */
                break;
        }

        if (line.is_comment || line.is_empty) {
            disposeLine(&line);
            continue;
        }

        switch (analyzeLine(line, assembler)) {
            case NEXT:
                break;
            case STOP:
                shouldStop = true;
                break;
            case ANALYZE_OUT_OF_MEMORY:
                shouldStop = true;
                logError("Out of memory!\n");
                return OUT_OF_MEMORY;
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
    updateDataLabels((int)assembler->IC, assembler->tables->labels_table);
    wordUpdateDecode((int) assembler->IC + 100, assembler->tables->words_map);

    return PARSE_SUCCESS;
}
