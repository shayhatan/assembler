#ifndef ASSEMBLER_FACTORY_H
#define ASSEMBLER_FACTORY_H

#include "../parsers/parse_types.h"

Entry createEntry(char *classification, /* symbol property */
                  int value /* IC + 100 || DC */);

#endif /*ASSEMBLER_FACTORY_H*/
