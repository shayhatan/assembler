//
// Created by User on 08/04/2024.
//

#ifndef ASSEMBLER_DECODE_TABLE_H
#define ASSEMBLER_DECODE_TABLE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "data_structures/map/map.h"
#include "words/types.h"

Map words_map;

static MapDataElement copyElement(MapDataElement existing) {
    word *clone;
    word *existing_word = existing;

    if (existing == NULL) {
        return NULL;
    }

    clone = malloc(sizeof(word));
    if (clone == NULL) {
        return NULL;
    }

    memcpy(clone,existing_word, sizeof(word));

    return clone;
}

static MapKeyElement copyKeyElement(MapKeyElement existing) {
    int *clone;
    int *existing_int = (int *) existing;
    if (existing == NULL) return NULL;

    clone = malloc(sizeof(int));
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
    int *x = key1;
    int *y = key2;
    return *x == *y ? 0 : 1;
}

void wordsMapInit () {
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

    ptr[15] = '\0';

    // If negative number, get two's complement
    if (x < 0) {
        x = 16384 + x; // 2^14 = 16384
    }

    // Convert the integer to binary
    for (i = 14; i >= 0; i--) {
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
                // If the input is invalid, keep it unchanged
                base4[i] = encrypted[i];
        }
    }
    base4[i] = '\0'; // Add null terminator
}

/* need to add doc */
MapResult wordUpdateDecode(int to_concat) {
    int *iter = mapGetFirst(words_map);
    int key;
    word *current_word;
    if (words_map == NULL) {
        return MAP_NULL_ARGUMENT;
    }

    while (iter != NULL && *iter < 100) {
        current_word = mapGet(words_map, iter);
        key = *iter + to_concat;
        mapPut(words_map, &key, current_word);
        mapRemove(words_map, iter);
        free(iter);
        iter = mapGetFirst(words_map);
    }

    if(!iter)
        free(iter);

    return MAP_SUCCESS;
}

void printWordsMap() {
    int * iter;
    word *current_word;
    MAP_FOREACH(int *, iter, words_map) {
        char buffer[15];

        current_word = mapGet(words_map, iter);
        base10ToBase2(current_word->print, buffer);
        printf("%d\t%s\n", *iter, buffer);
        free(iter);
    }
}

#endif //ASSEMBLER_DECODE_TABLE_H
