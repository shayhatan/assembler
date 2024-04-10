/*
 Created by User on 20/03/2024.
*/



#ifndef ASSEMBLER_FACTORY_H
#define ASSEMBLER_FACTORY_H

#include "parsers/parse_types.h"

entry createEntry(char *classification, /* symbol property */
                  int value /* IC + 100 || DC */) {
    entry e;

    e.value = value;
    e.classification = classification;
    e.wordsCounter = 0;

    return e;
}

#endif /*ASSEMBLER_FACTORY_H*/
