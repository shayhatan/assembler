/*
 Created by User on 08/04/2024.
*/

#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "types.h"
#include "../parsers/parse_types.h"
#include "../parsers/parsers_utils.h"
#include "../labels_table.h"
#include "../string_utils.h"
#include "../decode_table.h"
#include "decoders.h"


typedef enum OperandType {
    SOURCE,
    TARGET
} OperandType;

bool tryGetNumberFromConstantSymbolOrSting(Operand operand, int* result) {
    unsigned  int temp;
    if (isValidVariableString(operand)){ /* in this case the argument should be a .define symbol */
        if (getConstantByLabel(operand, &temp) != MAP_SUCCESS) {
            return false;
        }
        *result = (signed) temp;
        return true;
    }
    if (!tryGetNumber(operand, result)) {
        return false;
    }

    return true;
}

bool tryExtractListAccess(Operand operand, char *list_ptr, char* index_ptr) {
    int pos_of_array_start = indexOfChar(operand, '[');
    int pos_of_array_end = indexOfChar(operand, ']') - pos_of_array_start - 1;
    if (pos_of_array_start == INT_MAX || pos_of_array_end == INT_MAX) {
        return false;
    }
    duplicateStr(operand, list_ptr, pos_of_array_start);
    duplicateStr(operand+pos_of_array_start+1, index_ptr, pos_of_array_end);
    return  true;
}


MapResult decodeAndAddCommand(int *address, Opcode code, char operands[2][MAX_ARG_CHARS], int length) {
    word cmd_word;

    cmd_word.cmd.opcode = (int) code;
    cmd_word.cmd.decode=0;
    switch (length) {
        case 0:
            cmd_word.cmd.sourceOperand = 0;
            cmd_word.cmd.targetOperand = 0;
            break;
        case 1:
            cmd_word.cmd.targetOperand = getAddressingForOperand(operands[0]);
            cmd_word.cmd.sourceOperand = 0;
            break;
        case 2:
            cmd_word.cmd.sourceOperand = getAddressingForOperand(operands[0]);
            cmd_word.cmd.targetOperand = getAddressingForOperand(operands[1]);
            break;
        default:
            return MAP_ERROR;
    }
    cmd_word.cmd.unused=0;

    return addWord((*address)++, &cmd_word);
}


MapResult tryDecodeInstantOperand(Operand operand, word* word) {
    int s_temp;
    word->instant.decode = 0;
    if (!tryGetNumberFromConstantSymbolOrSting(operand+1, &s_temp)) {
        return MAP_ERROR;
    }
    word->instant.value = s_temp;
    return MAP_SUCCESS;
}

MapResult tryDecodeDirectOperand(Operand operand, word* word) {
    entry *label_entry = NULL;
    label_entry = get_data(operand);

    if (label_entry == NULL) {
        return MAP_ERROR;
    }

    if (strcmp(label_entry->classification, RAW_DOT_DATA) == 0 || strcmp(label_entry->classification, RAW_DOT_STRING) == 0 || strcmp(label_entry->classification, DOT_CODE) == 0) {
        /*if (label_entry->wordsCounter >1) {
            *//* error *//*
                    }*/
        word->direct.decode = RELATIVE_DECODING;
        word->direct.value = label_entry->value;
        return MAP_SUCCESS;
    }
    if (strcmp(label_entry->classification, DOT_EXTERNAL) == 0) {
        word->direct.decode = EXTERNAL_DECODING;
        word->direct.value = 0;
        return MAP_SUCCESS;
    }

    return MAP_SUCCESS;
}

MapResult tryDecodeConstantIndexOperand(int *address, Operand operand) {
    int s_temp;
    entry *label_entry = NULL;
    char list_name[MAX_ARG_CHARS];
    char index_name[MAX_ARG_CHARS];
    word array_word;
    word index_word;
    MapResult status;

    if (!tryExtractListAccess(operand, list_name, index_name)) {
        return MAP_ERROR;
    }

    label_entry = get_data(list_name);
    if (label_entry == NULL) {
        return MAP_ERROR;
    }

    array_word.direct.value = label_entry->value;
    array_word.direct.decode = strcmp(label_entry->classification, RAW_DOT_EXTERN) == 0 ? EXTERNAL_DECODING : RELATIVE_DECODING;

    status = addWord(*address,&array_word);
    if (status != MAP_SUCCESS) {
        return  status;
    }
    *address += 1;


    if (!tryGetNumberFromConstantSymbolOrSting(index_name, &s_temp)) {
        return MAP_ERROR;
    }

    index_word.instant.value = s_temp;
    index_word.instant.decode = 0;

    status = addWord(*address,&index_word);
    if (status != MAP_SUCCESS) {
        return  status;
    }
    *address += 1;

    return MAP_SUCCESS;
}

MapResult tryDecodeDirectRegisterOperand(Operand operand, word* word, OperandType type) {
    word->reg.decode = 0;
    word->reg.unused = 0;

    switch (type) {
        case SOURCE:
            word->reg.sourceOperand = (unsigned int)*(operand+1);
            word->reg.targetOperand = 0;
            break;
        case TARGET:
            word->reg.targetOperand = (unsigned int)*(operand+1);
            word->reg.sourceOperand = 0;
            break;
    }

    return MAP_SUCCESS;
}

/* we assume that this is called in the 2nd run where we already know that the lines are valid input */
MapResult decodeInstruction(int *address, Opcode code, char operands[2][MAX_ARG_CHARS], int length) {
    int i;
    word new_word;
    /* try to unify words_map if possible */
    OperandType type = length == 2 ? SOURCE : TARGET;
    MapResult status = decodeAndAddCommand(address, code, operands, length);

    if (status != MAP_SUCCESS) {
        return status;
    }

    if (length ==2 && getAddressingFlagForOperand(operands[0]) == directRegister &&
            getAddressingFlagForOperand(operands[1]) == directRegister) {
        new_word.reg.sourceOperand=(unsigned int)*(operands[0]+1);
        new_word.reg.targetOperand=(unsigned int)*(operands[1]+1);
        new_word.reg.unused = 0;
        new_word.reg.decode = ABSOLUTE_DECODING;
        status = addWord((*address)++,&new_word);
        if (status != MAP_SUCCESS) {
            return status;
        }
        return MAP_SUCCESS;
    }

    for (i = 0; i < length; i++) {
        enum Addressing addressing = getAddressingFlagForOperand(operands[i]);

        if (addressing == constantIndex) {
            status = tryDecodeConstantIndexOperand(address, operands[i]);
            if (status != MAP_SUCCESS) {
                return status;
            }
            type++;
            continue;
        }

        switch (addressing) {
            case instant:
                status = tryDecodeInstantOperand(operands[i], &new_word);
                break;
            case direct:
                status = tryDecodeDirectOperand(operands[i], &new_word);
                break;
            case directRegister:
                status = tryDecodeDirectRegisterOperand(operands[i], &new_word, type);
                break;
            default:
                return MAP_ERROR;
        }
        if (status != MAP_SUCCESS) {
            return status;
        }
        status = addWord((*address)++, &new_word);
        if (status != MAP_SUCCESS) {
            return status;
        }
        type++;
    }
    return MAP_SUCCESS;
}

MapResult decodeData(int *address, Arguments *args) {
    int i;
    int temp;
    word new_word;
    MapResult status = MAP_SUCCESS;

    for (i = 0; i < args->args_count; ++i) {
        if (!tryGetNumberFromConstantSymbolOrSting(args->args[i], &temp)) {
            return MAP_ERROR;
        }
        new_word.data.value = temp;
        status = addWord(*address, &new_word);
        if (status != MAP_SUCCESS) {
            return status;
        }
        *address += 1;
    }
    return MAP_SUCCESS;
}

MapResult decodeString(int * address, Arguments *args) {
    word new_word;
    char* ptr = args->args[0];
    MapResult status = MAP_SUCCESS;

    while(*ptr != '\0') {
        new_word.data.value = (int)*ptr;
        status = addWord(*address, &new_word);
        if (status != MAP_SUCCESS) {
            return status;
        }
        *address += 1;
        ptr++;
    }
    new_word.data.value = (int)*ptr;
    status = addWord(*address, &new_word);
    if (status != MAP_SUCCESS) {
        return status;
    }
    *address += 1;
    ptr++;
    return MAP_SUCCESS;
}

