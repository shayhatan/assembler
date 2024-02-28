//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

#include "../data_structures/types.h"
#include "../operands/types.h"

void addLabel(char *labelName, int value);

int getLabel(char *labelName);


#endif //ASSEMBLER_UTILS_H
