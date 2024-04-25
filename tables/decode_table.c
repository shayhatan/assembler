#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../data_structures/map/map.h"
#include "../words/types.h"
#include "../utils/memory.h"
#include "./decode_table.h"
#include "../utils/string_utils.h"
#include "labels_table.h"



void compileInstruction(char *result, const int *key_ptr, const Word *current_word);

static MapDataElement copyElement(MapDataElement existing) {
    Word *clone;
    Word *existing_word = existing;

    if (existing == NULL) {
        return NULL;
    }

    clone = allocateMemory(sizeof(Word));
    if (clone == NULL) {
        return NULL;
    }

    memcpy(clone, existing_word, sizeof(Word));

    return clone;
}

static MapKeyElement copyKeyElement(MapKeyElement existing) {
    int *clone;
    int *existing_int = (int *) existing;
    if (existing == NULL) return NULL;

    clone = allocateMemory(sizeof(int));
    if (clone == NULL) {
        return NULL;
    }

    *clone = *existing_int;
    return clone;
}

/** Type of function for deallocating a data element of the map */
static void cleanMapDataElements(MapDataElement disposable) {
    Word *disposed_word = disposable;
    free(disposed_word);
}

/** Type of function for deallocating a key element of the map */
static void cleanMapKeyElements(MapKeyElement key) {
    int *existing_int = (int *) key;
    free(existing_int);
}

static int compareKeyElements(MapKeyElement key1, MapKeyElement key2) {
    return *(int *) key1 - *(int *) key2;
}

/*void wordsMapInit() {
    if (words_map != NULL) return;
    words_map = mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                          compareKeyElements);
}*/

Map wordsMapCreate() {
    return mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                     compareKeyElements);
}

void wordsMapDispose(Map words_map) {
    if (words_map == NULL) return;
    mapDestroy(words_map);
}

MapResult addWord(int address, Word *word, Map words_map) {
    if (words_map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    if (mapContains(words_map, &address)) {
        return MAP_ITEM_ALREADY_EXISTS;
    }
    return mapPut(words_map, &address, word);
}

void base10ToBase2(int x, char *ptr) {
    int i;

    ptr[14] = '\0'; /* Change from ptr[15]*/

    /* Convert the integer to binary */
    for (i = 13; i >= 0; i--) {
        ptr[i] = (x & 1) ? '1' : '0';
        x >>= 1;
    }
}

void encryptedToBase4(char *encrypted, char *base4) {
    int i;

    for (i = 0; encrypted[i] != '\0'; i++) {
        switch (encrypted[i]) {
            case '*':
                base4[i] = '0';
                break;
            case '#':
                base4[i] = '1';
                break;
            case '%':
                base4[i] = '2';
                break;
            case '!':
                base4[i] = '3';
                break;
            default:
                /* If the input is invalid, keep it unchanged */
                base4[i] = encrypted[i];
        }
    }
    base4[i] = '\0'; /* Add null terminator */
}

void base4ToEncrypted(char *base4, char *encrypted) {
    int i;

    for (i = 0; base4[i] != '\0'; i++) {
        switch (base4[i]) {
            case '0':
                encrypted[i] = '*';
                break;
            case '1':
                encrypted[i] = '#';
                break;
            case '2':
                encrypted[i] = '%';
                break;
            case '3':
                encrypted[i] = '!';
                break;
            default:
                /* If the input is invalid, keep it unchanged */
                encrypted[i] = base4[i];
        }
    }
    encrypted[i] = '\0'; /* Add null terminator */
}

void binaryToBase4(const char *binary, char *base4) {
    int i;
    int binaryLength = strlen(binary);
    int base4Index = 0;
    int remainder = binaryLength % 2; /*To handle cases with odd number of digits*/
    int base4Digit = 0;

    /*Check if the binary length is odd and prepend a zero if necessary*/
    if (remainder != 0) {
        base4[base4Index++] = '0';
    }
    /* Iterate over the binary string */
    for (i = remainder; i < binaryLength; i += 2) {
        /* Convert each pair of bits to base-4 digit */
        base4Digit = (binary[i] - '0') * 2 +
                     (i + 1 < binaryLength ? binary[i + 1] - '0' : 0); /* Handle the case where binaryLength is odd*/
        base4[base4Index++] = base4Digit + '0'; /* Convert to character and store */
    }
    base4[base4Index] = '\0'; /* Null-terminate the base4 string */
}


MapResult wordUpdateDecode(int IC, Map words_map) {
    int *iter;
    int key;
    Word *current_word;
    if (words_map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    iter = mapGetFirst(words_map);
    /*use define (address starts from 100)*/
    while (iter != NULL && (*iter) < 100) {
        current_word = mapGet(words_map, iter);
        key = *iter + IC;
        mapPut(words_map, &key, current_word);
        mapRemove(words_map, iter);
        free(iter);
        iter = mapGetFirst(words_map);
    }

    if (iter)
        free(iter);

    return MAP_SUCCESS;
}

MapIterationResult getNextLine(char *result, Map words_map) {
    static int iter = -1;
    int *key_ptr = NULL;
    Word *current_word;

    if (words_map == NULL) {
        return UNDEFINED_MAP;
    }

    if (iter == -1) {
        key_ptr = mapGetFirst(words_map);
        if (key_ptr == NULL) {
            return NULL_NODE;
        }
        current_word = mapGet(words_map, key_ptr);
        iter++;
        compileInstruction(result, key_ptr, current_word);
        free(key_ptr);

        return SUCCESSFUL_ITERATION;
    }

    key_ptr = mapGetNext(words_map);
    if (key_ptr == NULL) { /* reached end of map */
        iter = -1;
        return ITERATION_FINISHED;
    }
    current_word = mapGet(words_map, key_ptr);
    iter++;
    compileInstruction(result, key_ptr, current_word);
    free(key_ptr);

    return SUCCESSFUL_ITERATION;
}

void compileInstruction(char *result, const int *key_ptr, const Word *current_word) {
    char base2_buffer[15];
    char base4_buffer[8];
    char encrypted_buffer[8];
    base10ToBase2(current_word->print, base2_buffer);
    binaryToBase4(base2_buffer, base4_buffer);
    base4ToEncrypted(base4_buffer, encrypted_buffer);

    sprintf(result, "%d\t%s", *key_ptr, encrypted_buffer);
}

int writeWordsMap(FILE *ob_file, Map words_map, Map labels_table, int IC, int DC) {
    char buffer[81] = {'\0'};
    MapIterationResult status;
    int size = 0; /* If empty we won't create a file*/

    getDCAndIC(buffer,labels_table, IC, DC);
    fprintf(ob_file, "%s\n", buffer);
    status = getNextLine(buffer, words_map);
    while (status == SUCCESSFUL_ITERATION) {
        ++size;
        printf("%s\n", buffer);
        fprintf(ob_file, "%s\n", buffer);
        resetString(buffer);
        status = getNextLine(buffer, words_map);
    }
    return size;
}
