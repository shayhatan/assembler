#ifndef FIRST_RUN_H
#define FIRST_RUN_H

#include "../parsers/parse_types.h"
#include <stdio.h>

typedef enum {
    STOP, NEXT, ANALYZE_OUT_OF_MEMORY
} AnalyzeStatus;

ParseResult run(FILE *srcFile);

#endif /* FIRST_RUN_H */