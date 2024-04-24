#ifndef ASSEMBLER_SECOND_RUN_H
#define ASSEMBLER_SECOND_RUN_H

#include <stdio.h>
#include "../parsers/parse_types.h"
#include "assembler.h"

enum ParseResult secondRun(FILE *srcFile, Assembler* assembler);

#endif /*ASSEMBLER_SECOND_RUN_H*/
