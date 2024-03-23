//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "./parsers/types.h"
#include "labels_table.h"
#include "./data_structures/linked_list/list.h"
#include "logs/utils.h"
#include "factory.h"
#include "parsers/utils.h"

unsigned int IC = 0, DC = 0;
bool errored;

input_line parseLine(char *line, int lineNumber) {
    input_line parsed_line;
    enum SentenceType sentence_type;

    parsed_line.lineNumber = lineNumber;
    parsed_line.isEOF = isEOF(line);
    if (parsed_line.isEOF) {
        /* we can stop here */
        return parsed_line;
    }

    sentence_type = getSentenceType(line);

    parsed_line.hasLabel = doesContainLabel(line);
    if (parsed_line.hasLabel) {
        parsed_line.label = getLabelValue(line);
    }

    switch (sentence_type) {
        case EMPTY:
        case COMMENT:
            return parsed_line;
        case DIRECTIVE:
            parsed_line.directive_props = getDirectiveProps(line);
            switch (parsed_line.directive_props) {
                case dot_data:
                    parsed_line.arguments->numbers = getArguments(line, NUMERIC_TYPE, PLURAL);
                    break;
                case dot_string:
                    parsed_line.arguments->strings = getArguments(line, STRING_TYPE, SINGLE);
                    break;
                case dot_external:
                    parsed_line.arguments->strings = getArguments(line, LABEL_TYPE, PLURAL);
                    break;
                case dot_entry:
                    parsed_line.arguments->strings = getArguments(line, LABEL_TYPE, SINGLE);
                    break;
                case dot_define:
                    parsed_line.arguments->numbers = getArguments(line, NUMERIC_TYPE, SINGLE);
                    break;
            }
            break;
        case INSTRUCTION:
            parsed_line.opcode = getOpcode(line);
            parsed_line.arguments = getOperationArguments(line);
            break;
        case CONSTANT_DEFINITION:
            break;
        case INVALID:
            /* todo: need to go into error flow here */
            break;
    }

    return parsed_line;
};

void disposeLine(input_line *line) {

    if (line->arguments != NULL) {
        /* todo we need to determine which of the following needs to be disposed as its a union type*/
        listDispose(line->arguments->numbers);
        listDispose(line->arguments->strings);
        free(line->arguments);
    }

    if (line->label != NULL)
        free(line->label);
};


enum analyze_status {
    STOP, NEXT
};

static input_line currentLine;

void countStringWords(unsigned int _, void *data) {
    char *strPtr = (char *) data;
    while (strPtr != NULL) {
        /* each character is one word */
        incrementLabelWordsCounter(currentLine.label);
        strPtr++;
    }
}

enum analyze_status analyze_line(input_line line) {
    if (line.isEOF) {
        return STOP;
    }

    if (line.directive_props & dot_define) {
        addLabel(line.label, createEntry(DOT_DEFINE, *(unsigned int *) getFirst(line.arguments->numbers)->value));
        return NEXT;
    }

    /* step 5 */
    if (line.directive_props & (dot_data | dot_string) && line.hasLabel) {
        entry *addedEntry;
        /* step 6 */
        if (addLabel(line.label, createEntry(DOT_DATA, DC)) != 0) {
            return NEXT;
        }
        /* increase DC according to arguments */
        if (line.directive_props & dot_string) {
            iterate(line.arguments->strings, countStringWords);
        }
        if (line.directive_props & dot_data) {
            incrementLabelWordsCounter(line.label);
        }

        /* it's impossible for addedEntry to be null in this case */
        addedEntry = get_data(line.label);
        /* step 7 */
        DC += addedEntry->wordsCounter;
        return NEXT;
    }

    /* step 8, 9 */
    if (line.directive_props & dot_external) {
        bulkAddExternalOperands(line.arguments->strings);
        return NEXT;
    }

    /* step 8, 10 */
    if ((line.directive_props & dot_entry) && line.hasLabel) {
        if (!addLabel(line.label, createEntry(DOT_CODE, IC + 100))) {
            return NEXT;
        }
    }

    /* step 11 */
    if (line.opcode < 0 || line.opcode > 15) {
        log_error("invalid operation %d", line.opcode);
        return NEXT;
    }

    /* step 12 + 13 */
    IC += /* L */ getOperationWordsCounter(&line);

    return NEXT;
}

int run(FILE *srcFile) {
    char buffer[81];

    int index = 0;
    while (fgets(buffer, 81, srcFile)) {
        bool shouldStop = false;
        input_line line = parseLine(buffer, index++);
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