#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H

#include "tables/tables.h"

typedef struct {
    Tables *tables;
    unsigned int IC;
    unsigned int DC;
    bool has_dot_ent;
} Assembler;

void assemblerInit(Assembler *assembler);

#endif /*ASSEMBLER_ASSEMBLER_H*/
