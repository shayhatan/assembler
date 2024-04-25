
#ifndef ASSEMBLER_LINE_UTILS_H
#define ASSEMBLER_LINE_UTILS_H
#include "parse_types.h"


enum ParseResult parseLine(char *line, int line_number, input_line *result);

void resetLine(input_line *line);

void disposeLine(input_line *line);

#endif /* ASSEMBLER_LINE_UTILS_H */
