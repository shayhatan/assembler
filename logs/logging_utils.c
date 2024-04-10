
/*
 Created by User on 17/03/2024.
*/


#ifndef ASSEMBLER_UTILS_LOGS_H
#define ASSEMBLER_UTILS_LOGS_H


#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "../string_utils.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

extern bool errored;

char prefix[81];
int line;

void setLogLineContext(int line_number, char *line_text) {
    int index = min(indexOfChar(line_text, '\n'), indexOfChar(line_text, '\0'));
    line = line_number;
    duplicateStr(line_text, prefix, index);
}

void log_error(char *error_msg, ...) {
    va_list lst;
    va_start(lst, error_msg);

    printf("[ERROR][line-%d] \"%s\" - failed by: ", line, prefix);

    while (*error_msg != '\0') {
        if (*error_msg != '%') {
            putchar(*error_msg);
            error_msg++;
            continue;
        }
        error_msg++;
        switch (*error_msg) {
            case 'd':
                /*putchar(va_arg(lst, int));*/
                printf("%d", va_arg(lst, int));
                break;
            case 's':
                printf("%s", va_arg(lst, char*));
                /*fputs(va_arg(lst, char *), stdout);*/
                break;
            case 'c':
                putchar(va_arg(lst, int));
                break;
        }
        error_msg++;
    }
    va_end(lst);
}

#endif /*ASSEMBLER_UTILS_LOGS_H*/
