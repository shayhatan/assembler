//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_UTILS_LOGS_H
#define ASSEMBLER_UTILS_LOGS_H


#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

extern bool errored;

void log_error(char *error_msg, ...) {
    char *s;
    va_list lst;
    va_start(lst, error_msg);
    while (*error_msg != '\0') {
        if (*error_msg != '%') {
            putchar(*error_msg);
            error_msg++;
            continue;
        }
        error_msg++;
        switch (*error_msg) {
            case 'd':
                putchar(va_arg(lst, int));
                break;
            case 's':
                printf("%s", va_arg(lst, char*));
//                fputs(va_arg(lst, char *), stdout);
                break;
            case 'c':
                putchar(va_arg(lst, int));
                break;
        }
    }
}

#endif //ASSEMBLER_UTILS_LOGS_H
