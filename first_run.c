//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <string.h>
#include "./parsers/types.h"
#include "./data_structures/table/types.h"
#include "./data_structures/table/utils.h"
#include "./data_structures/linked_list//utils.h"

unsigned int IC = 0, DC = 0;


input_line parseLine(char *line) {

};

int run(FILE *srcFile) {
    char buffer[81];
    table entries;
    table commands;
    list externals;
    init_table(&entries);
    init_table(&commands);
    init_list(&externals);

    while (fgets(buffer, 81, srcFile)) {
        entry newEntry;

        input_line line = parseLine(buffer);
        if (line.isEOF) {
            break;
        }
        if (line.labelProps & (dot_data | dot_string) & line.hasLabel) {
            if (getValue(&entries, line.label) != NULL) {
                /*perror("label %s already exists", line->label);*/
                continue;
            }
            newEntry.classification = DOT_DATA;
            newEntry.value = DC;
            setValue(&entries, line.label, &newEntry);

            /* increase DC according to arguments */
            switch (line.labelType) {
                case integer:
                    DC += sizeof(int);
                case string:
                    DC += strlen(*(line.args));
                case character:
                    DC += 1;
            }
            continue;
        }

        if (line.labelProps & dot_external) {
            char *arg = *line.args;
            while (arg != NULL) { /* might be better to use a counter here instead */
                addLast(&externals, arg++);
            }
            continue;
        }
        if (line.labelProps & dot_entry) {
            if (line.hasLabel) {
                if (getValue(&entries, line.label) != NULL) {
                    perror("label already exists");
                    continue;
                }
                newEntry.classification = DOT_CODE;
                newEntry.value = IC + 100;
                setValue(&entries, line.label, &newEntry);
            }

        }
        /* step 11 in course notebook */
        if (getValue(&commands, line.label) == NULL) {
            perror("Expected command to exist in commands table");
            continue;
        }
/*        IC += IC + L; */
    }

    /*if (errored) {
        return 1000; / custom error code /
    }*/

    /*update all symbols with data classification to IC + 100 */
    return 0;
}