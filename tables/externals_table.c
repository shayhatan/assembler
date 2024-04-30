
#include <stdio.h>
#include "../utils/memory.h"
#include "../utils/string_utils.h"
#include "../data_structures/map/map.h"


void compileExternal(char *result, const int *line_number, char *current_external) {
    sprintf(result, "%s %d", current_external, *line_number);
}

static MapDataElement copyElement(MapDataElement existing) {
    char *clone;
    char *existing_word = existing;

    if (existing == NULL) {
        return NULL;
    }

    clone = allocatedDuplicateString(existing_word);

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
    char **disposed_word = disposable;
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

/*
void externalsMapInit() {
    if (externals_map != NULL) return;
    externals_map = mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                              compareKeyElements);
}
*/

Map externalsMapCreate() {
    return mapCreate(copyElement, copyKeyElement, cleanMapDataElements, cleanMapKeyElements,
                     compareKeyElements);
}

void externalsMapDispose(Map externals_map) {
    if (externals_map == NULL) return;
    mapDestroy(externals_map);
}

MapResult addExternal(int address, char *label, Map externals_map) {
    if (externals_map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    if (mapContains(externals_map, &address)) {
        return MAP_ITEM_ALREADY_EXISTS;
    }
    return mapPut(externals_map, &address, label);
}


MapIterationResult getNextExternal(char *result, Map externals_map) {
    static int iter = -1;
    int *key_ptr = NULL;
    char *external_label;

    if (externals_map == NULL) {
        return UNDEFINED_MAP;
    }

    if (iter == -1) {
        key_ptr = mapGetFirst(externals_map);
        if (key_ptr == NULL) {
            return NULL_NODE;
        }
        external_label = mapGet(externals_map, key_ptr);
        iter++;
        compileExternal(result, key_ptr, external_label);
        free(key_ptr);

        return SUCCESSFUL_ITERATION;
    }

    key_ptr = mapGetNext(externals_map);
    if (key_ptr == NULL) { /* reached end of map */
        iter = -1;
        return ITERATION_FINISHED;
    }
    external_label = mapGet(externals_map, key_ptr);
    iter++;
    compileExternal(result, key_ptr, external_label);
    free(key_ptr);

    return SUCCESSFUL_ITERATION;
}

bool isEmptyExternals(Map externals_map) {
    return mapGetSize(externals_map) == 0;
}

int writeExternals(FILE *ext_file, Map externals_map) {
    int size = 0; /* if size is 0  by the end of the func we wont create ext_file*/
    char buffer[81] = {'\0'};
    MapIterationResult status = getNextExternal(buffer, externals_map);
    printf("===========Print Externals==============\n");
    while (status == SUCCESSFUL_ITERATION) {
        ++size;
        printf("%s\n", buffer);
        fprintf(ext_file, "%s\n", buffer);
        resetString(buffer);
        status = getNextExternal(buffer, externals_map);
    }
    return size;
}
