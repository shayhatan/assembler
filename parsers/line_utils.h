
#ifndef ASSEMBLER_LINE_UTILS_H
#define ASSEMBLER_LINE_UTILS_H
#include "parse_types.h"


/** Parses a buffer into an assembly line metadata */
enum ParseResult parseLine(char *line, int line_number, InputLine *result);

/** Sets line metadata to default */
void resetLine(InputLine *line);

/** Clears line metadata */
void disposeLine(InputLine *line);

#endif /* ASSEMBLER_LINE_UTILS_H */
