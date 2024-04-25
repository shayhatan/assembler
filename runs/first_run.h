#ifndef FIRST_RUN_H
#define FIRST_RUN_H

#include "assembler_types.h"
#include "../parsers/parse_types.h"
#include <stdio.h>

typedef enum {
    STOP, NEXT, ANALYZE_OUT_OF_MEMORY
} AnalyzeStatus;

ParseResult run(FILE *src_file, Assembler* assembler);

#endif /* FIRST_RUN_H */
