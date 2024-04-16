/*
 Created by User on 08/04/2024.
*/

#ifndef ASSEMBLER_DECODE_TABLE_H
#define ASSEMBLER_DECODE_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_structures/map/map.h"
#include "words/types.h"
#include "preprocessor/helper/helper.h"

Map words_map;

static MapDataElement copyElement(MapDataElement existing) {
    word *clone;
    word *existing_word = existing;

    if (existing == NULL) {
        return NULL;
    }

    clone = allocateMemory(sizeof(word));
    if (clone == NULL) {
        return NULL;
    }

    memcpy(clone, existing_word, sizeof(word));

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
    word *disposed_word = disposable;
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

void wordsMapInit() {
    if (words_map != NULL) return;
    words_map = mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                          compareKeyElements);
}

void wordsMapDispose() {
    if (words_map == NULL) return;
    mapDestroy(words_map);
}

MapResult addWord(int address, word *word) {
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
        base4Digit = (binary[i] - '0') * 2 + (i + 1 < binaryLength ? binary[i + 1] - '0' : 0); /* Handle the case where binaryLength is odd*/
        base4[base4Index++] = base4Digit + '0'; /* Convert to character and store */
    }
    base4[base4Index] = '\0'; /* Null-terminate the base4 string */
}


MapResult wordUpdateDecode(int IC) {
    int *iter;
    int key;
    word *current_word;
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

void printWordsMap() {
    int *iter;
    word *current_word;
    MAP_FOREACH(int *, iter, words_map) {
        char base2_buffer[15];
        char base4_buffer[8];
        char encrypted_buffer[8];

        current_word = mapGet(words_map, iter);

        base10ToBase2(current_word->print, base2_buffer);
        binaryToBase4(base2_buffer, base4_buffer);
        base4ToEncrypted(base4_buffer, encrypted_buffer);

        printf("%04i\t%s\n", *iter, encrypted_buffer);
        free(iter);
    }

}


#endif /*ASSEMBLER_DECODE_TABLE_H*/

