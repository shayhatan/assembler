#ifndef ASSEMBLER_UTILS_LOGS_H
#define ASSEMBLER_UTILS_LOGS_H


#include <stdbool.h>

void setLogLineContext(int line_number, char *line_text, char* ctx);

void logError(char *error_msg, ...);

#endif /*ASSEMBLER_UTILS_LOGS_H*/
