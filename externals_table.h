//
// Created by User on 20/03/2024.
//

#ifndef ASSEMBLER_EXTERNALS_TABLE_H
#define ASSEMBLER_EXTERNALS_TABLE_H

#include "data_structures/linked_list/types.h"

typedef char *Operand;

int addExternalOperand(Operand o);

int bulkAddExternalOperands(list *o);

#endif //ASSEMBLER_EXTERNALS_TABLE_H
