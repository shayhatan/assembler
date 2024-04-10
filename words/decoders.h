/*
 Created by User on 09/04/2024.
*/


#ifndef ASSEMBLER_DECODERS_H
#define ASSEMBLER_DECODERS_H

#include "../data_structures/map/map.h"
#include "../parsers/parse_types.h"

#define ABSOLUTE_DECODING 0
#define EXTERNAL_DECODING 1
#define RELATIVE_DECODING 2

MapResult decodeInstruction(int *address, Opcode code, char operands[2][MAX_ARG_CHARS], int length);

MapResult decodeData(int *address, Arguments *args);

MapResult decodeString(int *address, Arguments *args);

#endif /*ASSEMBLER_DECODERS_H*/
