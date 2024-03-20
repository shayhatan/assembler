//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <string.h>
#include "./parsers/types.h"
#include "labels_table.h"
#include "./data_structures/linked_list/list.h"
#include "logs/utils.h"
#include "factory.h"
#include "parsers/utils.h"

unsigned int IC = 0, DC = 0;
bool errored;

input_line parseLine(char *line) {

};

input_line disposeLine(input_line *line) {

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

    /* step 5 */
    if (line.labelProps & (dot_data | dot_string) && line.hasLabel) {
        entry *addedEntry;
        /* step 6 */
        if (addLabel(line.label, createEntry(DOT_DATA, DC)) != 0) {
            return NEXT;
        }
        /* increase DC according to arguments */
        if (line.labelProps & dot_string) {
            iterate(line.arguments->strings, countStringWords);
        }
        if (line.labelProps & dot_data) {
            incrementLabelWordsCounter(line.label);
        }

        /* it's impossible for addedEntry to be null in this case */
        addedEntry = get_data(line.label);
        /* step 7 */
        DC += addedEntry->wordsCounter;
        return NEXT;
    }

    /* step 8, 9 */
    if (line.labelProps & dot_external) {
        bulkAddExternalOperands(line.arguments->strings);
        return NEXT;
    }

    /* step 8, 10 */
    if ((line.labelProps & dot_entry) && line.hasLabel) {
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

    while (fgets(buffer, 81, srcFile)) {
        bool shouldStop = false;
        input_line line = parseLine(buffer);
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