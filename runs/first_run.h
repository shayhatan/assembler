/*
 Created by User on 17/03/2024.
*/

#include "./parsers/parse_types.h"

typedef enum {
    STOP, NEXT, ANALYZE_OUT_OF_MEMORY
} AnalyzeStatus;


enum ParseResult run(FILE *srcFile);
