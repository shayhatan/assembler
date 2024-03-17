//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_PARSERS_TYPES_H
#define ASSEMBLER_PARSERS_TYPES_H

#include <stdbool.h>
#include "../words/consts.h"

enum labelType {
    integer, character, string
};

enum symbol_flags {
    dot_data = 1, dot_string = 2, dot_external = 4, dot_entry = 8
};

/* symbol properties (symbol-table metadata) */
char *DOT_CODE = ".code";
char *DOT_EXTERNAL = ".external";
char *DOT_DATA = ".data";

typedef struct {
    char *classification; /* symbol property */
    unsigned int value; /* IC + 100 || DC */
} entry;

typedef struct {
    bool isEOF;
    bool isAssignment; /* is a .string/.data */
    bool hasLabel; /* does begin with a symbol */
    char *label;
    char **args; /* consider using other data type */
    enum labelType labelType;
    enum symbol_flags labelProps;
    enum opcode opcode;
} input_line;


#endif //ASSEMBLER_PARSERS_TYPES_H
