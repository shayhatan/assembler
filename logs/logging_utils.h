//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_UTILS_LOGS_H
#define ASSEMBLER_UTILS_LOGS_H


#include <stdarg.h>
#include <stdbool.h>

extern bool errored;

void setLogLineContext(int line_number, char *line_text);

void log_error(char *error_msg, ...);

#endif //ASSEMBLER_UTILS_LOGS_H
