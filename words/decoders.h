#ifndef ASSEMBLER_DECODERS_H
#define ASSEMBLER_DECODERS_H

#include "../parsers/parse_types.h"
#include "../data_structures/map/map.h"

#define ABSOLUTE_DECODING 0
#define EXTERNAL_DECODING 1
#define RELATIVE_DECODING 2

/**
 * @brief Decodes an instruction opcode and its operands.
 *
 * @param address Pointer to the current address in memory.
 * @param code The opcode of the instruction.
 * @param operands Array of strings representing the operands.
 * @param length Number of operands.
 * @param labels_table Map containing labels and their addresses.
 * @param words_map Map containing decoded words.
 * @param externals_table Map containing external references.
 * @return MapResult indicating success or failure.
 */
MapResult decodeInstruction(int *address, Opcode code, char operands[2][DEF_MAX_ARG_CHARS], int length, Map labels_table, Map words_map, Map externals_table);

/**
 * @brief Decodes a .data declaration and adds it to memory.
 *
 * @param address Pointer to the current address in memory.
 * @param args Arguments containing the data to be decoded.
 * @param labels_table Map containing labels and their addresses.
 * @param words_map Map containing decoded words.
 * @return MapResult indicating success or failure.
 */
MapResult decodeData(int *address, Arguments *args, Map labels_table, Map words_map);

/**
 * @brief Decodes a .string declaration and adds it to memory.
 *
 * @param address Pointer to the current address in memory.
 * @param args Arguments containing the string to be decoded.
 * @param words_map Map containing decoded words.
 * @return MapResult indicating success or failure.
 */
MapResult decodeString(int * address, Arguments *args, Map words_map);
#endif /*ASSEMBLER_DECODERS_H*/
