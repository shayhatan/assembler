//
// Created by User on 20/03/2024.
//


#ifndef ASSEMBLER_FACTORY_H
#define ASSEMBLER_FACTORY_H

#include "parsers/types.h"

entry createEntry(char *classification, /* symbol property */
                  unsigned int value /* IC + 100 || DC */);


List createIntegerList();

List createStringList();

#endif //ASSEMBLER_FACTORY_H
