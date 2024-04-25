
#ifndef ASSEMBLER_FACTORY_H
#define ASSEMBLER_FACTORY_H

#include "../parsers/parse_types.h"

Entry createEntry(char *classification, /* symbol property */
                  int value /* IC + 100 || DC */) {
    Entry e;

    e.value = value;
    e.classification = classification;
    e.words_counter = 0;
    e.is_entry = false;

    return e;
}

#endif /*ASSEMBLER_FACTORY_H*/
