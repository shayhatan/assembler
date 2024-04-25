
#ifndef ASSEMBLER_LINE_UTILS_H
#define ASSEMBLER_LINE_UTILS_H
#include "parse_types.h"


enum ParseResult parseLine(char *line, int line_number, InputLine *result);

void resetLine(InputLine *line);

void disposeLine(InputLine *line);

#endif /* ASSEMBLER_LINE_UTILS_H */
