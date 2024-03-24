//
// Created by User on 17/03/2024.
//

#ifndef ASSEMBLER_PARSERS_TYPES_H
#define ASSEMBLER_PARSERS_TYPES_H

#include <stdbool.h>
#include "../words/consts.h"
#include "data_structures/linked_list/list.h"

enum Addressing {
    instant = 1,
    direct = 2,
    indirectRegister = 4,
    directRegister = 8,
};

enum DirectiveProps {
    dot_data = 1, dot_string = 2, dot_external = 4, dot_entry = 8, dot_define = 16
};

typedef char *Operand;


/* symbol properties (symbol-table metadata) */
char *DOT_CODE = ".code";
char *DOT_DATA = ".data";
char *DOT_EXTERNAL = ".external";
char *DOT_DEFINE = ".mdefine";

typedef struct {
    char *classification; /* symbol property */
    unsigned int value; /* IC + 100 || DC */
    unsigned int wordsCounter;
} entry;


typedef union {
    List numbers;
    List strings;
} Arguments;

typedef struct {
    bool isEOF;
    bool hasLabel; /* does begin with a symbol */
    char *label;
    Arguments *arguments;
    enum DirectiveProps directive_props;
    enum opcode opcode;
    int lineNumber;
} input_line;

enum SentenceType {
    EMPTY, COMMENT, DIRECTIVE, INSTRUCTION, CONSTANT_DEFINITION,
    /* reserved for collisions or invalid categorization during parsing */
    INVALID
};

enum ArgumentType {
    NUMERIC_TYPE, STRING_TYPE, LABEL_TYPE
};

enum ArgumentsCountType {
    SINGLE, PLURAL
};


#endif //ASSEMBLER_PARSERS_TYPES_H
