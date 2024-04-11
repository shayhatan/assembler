/*
 Created by User on 17/03/2024.
*/

#include "./parsers/parse_types.h"

enum analyze_status {
    STOP, NEXT, ANALYZE_OUT_OF_MEMORY
};


enum ParseResult run(FILE *srcFile);
