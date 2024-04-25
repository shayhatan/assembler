#ifndef ASSEMBLER_ASSEMBLER_UTILS_H
#define ASSEMBLER_ASSEMBLER_UTILS_H

#include "../tables/tables.h"

typedef struct {
    Tables *tables;
    unsigned int IC;
    unsigned int DC;
    bool has_dot_ent;
} Assembler;

MapResult assemblerInit(Assembler *assembler);

void assemblerDispose(Assembler *assembler);

#endif /*ASSEMBLER_ASSEMBLER_UTILS_H*/
