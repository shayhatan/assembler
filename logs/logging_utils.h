#ifndef ASSEMBLER_UTILS_LOGS_H
#define ASSEMBLER_UTILS_LOGS_H


#include <stdbool.h>

extern bool errored;

void setLogLineContext(int line_number, char *line_text);

void logError(char *error_msg, ...);

#endif /*ASSEMBLER_UTILS_LOGS_H*/
