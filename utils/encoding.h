//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_ENCODING_H
#define ASSEMBLER_ENCODING_H

/* Converts a string to base4 */
int toQuaternaryBase(char *src, char *transformed);

/* encodes a base4 string according to the specs */
int encodeQuaternaryBase(char *src, char *transformed);

#endif //ASSEMBLER_ENCODING_H
