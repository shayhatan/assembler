//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <string.h>
#include "./parsers/types.h"
#include "labels_table.h"
#include "./data_structures/linked_list/utils.h"
#include "logs/utils.h"

unsigned int IC = 0, DC = 0;
bool errored;

input_line parseLine(char *line) {

};

enum analyze_status {
    stop, next
};

enum analyze_status analyze_line(input_line line, list *externals) {
    entry newEntry;
    char *label = line.labels[0];
    /* L */
    int wordCounter = 0;
    if (line.isEOF) {
        return stop;
    }
    if (line.labelProps & (dot_data | dot_string) & line.hasLabel) {
        entry *addedEntry;
        newEntry.classification = DOT_DATA;
        newEntry.value = DC;

        if (addLabel(label, newEntry)) {
            return next;
        }

        /* increase DC according to arguments */
        if (line.labelProps & dot_string) {
            char *c = line.arg;
            /* each character is one word */
            while (c != NULL) {
                incrementLabelWordsCounter(label);
                c++;
            }
            addedEntry = get_data(label);
            if (addedEntry != NULL) {
                DC += addedEntry->wordsCounter;
            }
        }
        if (line.labelProps & dot_data) {
            incrementLabelWordsCounter(label);
            DC += sizeof(int);
        }
        return next;
    }

    if (line.labelProps & dot_external) {
        while (label != NULL) { /* might be better to use a counter here instead */
            addLast(externals, label++);
        }
        return next;
    }
    if (line.labelProps & dot_entry) {
        if (line.hasLabel) {
            newEntry.classification = DOT_CODE;
            newEntry.value = IC + 100;
            addLabel(label, newEntry);
        }

    }
    /* step 11 in course notebook */
    if (line.opcode < 0 || line.opcode > 15) {
        log_error("invalid opcode %d", line.opcode);
    }
    IC += IC + wordCounter;
}

int run(FILE *srcFile) {
    char buffer[81];
    list externals;
    init_list(&externals);

    while (fgets(buffer, 81, srcFile)) {
        input_line line = parseLine(buffer);
        analyze_line(line, &externals);
    }

    if (errored) {
        return 1000; /* custom error code */
    }

    /*update all symbols with data classification to IC + 100 */
    return 0;
}