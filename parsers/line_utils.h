/*
 Created by User on 09/04/2024.
*/
#include "./parse_types.h"

#ifndef ASSEMBLER_LINE_UTILS_H
#define ASSEMBLER_LINE_UTILS_H


enum ParseResult parseLine(char *line, int lineNumber, input_line *result);

void resetLine(input_line *line);

void disposeLine(input_line *line);

#endif /* ASSEMBLER_LINE_UTILS_H */