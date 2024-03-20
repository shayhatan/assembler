//
// Created by User on 20/03/2024.
//


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parsers/types.h"
#include "logs/utils.h"
#include "./utils.h"
#include "../data_structures/linked_list/list.h"

const int TWO_OPERANDS_OPERATIONS[] = {mov, cmp, add, sub, lea};
const int ONE_OPERAND_OPERATIONS[] = {not, clr, inc, dec, jmp, bne, red, prn, jsr};
const int ZERO_OPERANDS_OPERATIONS[] = {rts, stop};


/* returns an enum flag */
int getAllowedSourceOperandAddressingsByOpcode(enum opcode op) {
    if (op == lea) {
        return direct;
    }
    if (op == mov || op == cmp || op == add || op == sub) {
        return instant |
               direct |
               indirectRegister |
               directRegister; /* all addressings are valid */
    }
    return 0; /* no source operand thus no addressings */
}

/* returns an enum flag */
int getAllowedTargetOperandAddressingsByOpcode(enum opcode op) {
    if (op == cmp || op == prn) {
        return instant |
               direct |
               indirectRegister |
               directRegister; /* all addressings are valid */
    }
    if (op == jsr) {
        return direct |
               indirectRegister;
    }
    if (op == rts || op == stop) {
        return 0; /* no target operand thus no addressings */
    }
    return direct |
           indirectRegister |
           directRegister;
}

int getAmountOfOperandsByOperation(enum opcode code) {
    if (code < 5) {
        return 2;
    }
    if (code < 14) {
        return 1;
    }
    return 0;
}


bool isRegisterAddressing(Operand operand) {
    return ((operand[0] == 'r' && '0' <= operand[1] && operand[1] <= '7' && operand[2] == '\0') ? true : false);
}

bool isIndirectRegisterAddressing(Operand operand) {
    return (operand[0] == '*') && isRegisterAddressing(operand + 1);
}

bool isInstantAddressing(Operand operand) {
    char *endPtr;
    if (operand[0] != '#') return false;

    strtol(operand, &endPtr, 10);
    return endPtr == operand || *endPtr != '\0';
}

bool isDirectAddressing(Operand operand) {
    char *current = operand;

    /* must not be a register */
    if (isRegisterAddressing(operand)) {
        return false;
    }

    /* must begin with a letter */
    if (!isalpha(*current)) {
        return false;
    }
    while (current != NULL) {
        /* only alphanumeric letters are allowed */
        if (!isalnum(*current)) {
            return false;
        }
        current++;
    }
    return true;
}

enum Addressing getAddressingForOperand(Operand operand) {
    if (isInstantAddressing(operand)) return instant;
    if (isDirectAddressing(operand)) return direct;
    if (isIndirectRegisterAddressing(operand)) return indirectRegister;
    if (isRegisterAddressing(operand)) return directRegister;

    return -1;
}

enum Destination {
    source, target
};

bool isAddressingValid(enum Addressing operandsAddressing, enum opcode opcode, enum Destination destination) {
    if (operandsAddressing == -1) {
        log_error("Invalid addressing, operands%s, doesn't have a defined addressing", operandsAddressing);
        return false;
    }

    /* is the translated addressing forbidden for the operand of this operation? */
    switch (destination) {
        case source:
            if (!(operandsAddressing & getAllowedSourceOperandAddressingsByOpcode(opcode))) {
                log_error("Invalid addressing, operand %s, is not allowed for opcode %d as source addressing",
                          operandsAddressing, opcode);
                return false;
            }
            break;
        case target:
            if (!(operandsAddressing & getAllowedTargetOperandAddressingsByOpcode(opcode))) {
                log_error("Invalid addressing, operand %s, is not allowed for opcode %d as target addressing",
                          operandsAddressing, opcode);
                return false;
            }
    }
    return true;
}

int getOperationWordsCounter(input_line *line) {
    enum Addressing operandsAddressing[2] = {-1};
    int argsCount;
    int amountOfOperands = getAmountOfOperandsByOperation(line->opcode);

    argsCount = listLength(line->arguments->strings);
    if (argsCount != amountOfOperands) {
        log_error("Arguments count %d does not match expected amount of operands %d", amountOfOperands);
        return -1;
    }

    switch (amountOfOperands) {
        case 0:
            return 0;
        case 1:
            operandsAddressing[0] = getAddressingForOperand(getNth(line->arguments->strings, 1)->value);
            /* single operand operations always involve a target operand */
            if (!isAddressingValid(operandsAddressing[0], line->opcode, target)) {
                return -1;
            }
            return 1;
        case 2:
            operandsAddressing[0] = getAddressingForOperand(getNth(line->arguments->strings, 1)->value);
            operandsAddressing[1] = getAddressingForOperand(getNth(line->arguments->strings, 2)->value);
            if (!isAddressingValid(operandsAddressing[0], line->opcode, source) ||
                !isAddressingValid(operandsAddressing[1], line->opcode, target)) {
                return -1;
            }
            /* both addressings are valid, check if they are different */
            /* yes --> we need 2 words, otherwise 1 word */
            return operandsAddressing[0] != operandsAddressing[1] ? 2 : 1;
        default:
            return -2; /* unreachable */
    }
}