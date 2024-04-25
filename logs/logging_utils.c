#ifndef ASSEMBLER_UTILS_LOGS_H
#define ASSEMBLER_UTILS_LOGS_H


#include "../utils/string_utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


#define min(a, b) (((a) < (b)) ? (a) : (b))

char prefix[81];
char context[81];
int line;

void setLogLineContext(int line_number, char *line_text, char* ctx) {
    int index = min(indexOfChar(line_text, '\n'), indexOfChar(line_text, '\0'));
    line = line_number;
    duplicateStr(line_text, prefix, index);
    strcpy(context, ctx);
}

void logError(char *error_msg, ...) {
    va_list lst;
    char* c_ptr;
    va_start(lst, error_msg);

    fprintf(stderr, "[ERROR][context=%s][#line=%d]: \"%s\" - failed by - ", context, line, prefix);

    while (*error_msg != '\0') {
        if (*error_msg != '%') {
            fprintf(stderr, "%c", *error_msg);
            error_msg++;
            continue;
        }
        error_msg++;
        switch (*error_msg) {
            case 'd':
                fprintf(stderr, "%d", va_arg(lst, int));
                break;
            case 's':
                c_ptr = va_arg(lst, char*);
                if (c_ptr == NULL) {
                    fprintf(stderr, "NULL");
                }
                else {
                    fprintf(stderr, "%s", c_ptr);
                }
                break;
            case 'c':
                fprintf(stderr, "%c", va_arg(lst, int));
                break;
        }
        error_msg++;
    }
    va_end(lst);
}

#endif /*ASSEMBLER_UTILS_LOGS_H*/
