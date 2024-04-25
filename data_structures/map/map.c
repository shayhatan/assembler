#include <stdlib.h>
#include <assert.h>
#include "map.h"
#include "../../utils/memory.h"

#define NEG (-1)
#define EQUAL 0
#define EMPTY NULL
#define EMPTY_THEN_NODE_IS_HEAD NULL
#define NODE_IS_TAIL NULL
#define END_OF_LIST NULL
#define EMPTY_MAP NULL

typedef struct Node_t {
    MapKeyElement key;
    MapDataElement data;
    struct Node_t *next;
} *Node;

typedef struct Map_t {
    Node head;
    Node iterator;

    /* for the work perfectly in a generic manner we need to provide it functions */
    copyMapDataElements copyData;
    copyMapKeyElements copyKey;
    freeMapDataElements freeData;
    freeMapKeyElements freeKey;
    compareMapKeyElements cmpKey;
} Map_t;

/** ------------------- STATICS FUNCTIONS --------------------------**/


/** Deallocates given Node*/
static void destroyNode(Map map, Node node) {
    map->freeKey(node->key);
    map->freeData(node->data);
    free(node);
}

/** nodeCreate: creating a Node of map by given key and value,
 *  the key and value must be reassign.
 *
 * @param map -  the map is used for the function (copy,free)
 * @param key -  A key to reassign(a copy) to the Node
 * @param data - Data to reassign(a copy) to the Node
 * @return
 *               NULL - If memory allocation, (assuming VALID values (assertion used))
 *               otherwise - new Node of map with the copy
 */
static Node nodeCreate(Map map, MapKeyElement key, MapDataElement data) {
    Node new_node;
    assert(key && data && map);
    new_node = allocateAndCountMemory(sizeof(*new_node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->key = map->copyKey(key);
    if (new_node->key == NULL) {
        free(new_node);
        return NULL;
    }
    new_node->data = map->copyData(data);
    if (new_node->data == NULL) {
        map->freeKey(key);
        free(new_node);
        return NULL;
    }
    new_node->next = NULL;
    return new_node;
}

/** insert: insert and sort a target Node of map the the map
 *
 * @param map Target map to insert
 * @param node Target Node to insert
 *        all the values must be checked before sent, means VALID
 */
static void insert(Map map, Node *node) {
    Node prev = EMPTY;
    Node iter = map->head;
    if (iter == EMPTY_MAP) {
        map->head = (*node);
        return;
    }
    while (iter != END_OF_LIST) {
        if (map->cmpKey(iter->key, (*node)->key) > 0) { /* left is smaller */
            if (prev == EMPTY_THEN_NODE_IS_HEAD) {
                map->head = (*node);
                (*node)->next = iter;
                return;
            }
            prev->next = (*node);
            (*node)->next = iter;
            return;
        }
        prev = iter;
        iter = iter->next;
    }
    assert(iter == NULL);
    prev->next = (*node);
}


Map mapCreate(copyMapDataElements copyData,
              copyMapKeyElements copyKey,
              freeMapDataElements freeData,
              freeMapKeyElements freeKey,
              compareMapKeyElements cmpKey
) {
    Map map = allocateMemory(sizeof(*map));
    if (copyData == NULL || copyKey == NULL || freeData == NULL
        || freeKey == NULL || cmpKey == NULL) {
        return NULL;
    }

    if (map == NULL) {
        return NULL;
    }

    map->copyData = copyData;
    map->copyKey = copyKey;
    map->freeData = freeData;
    map->freeKey = freeKey;
    map->cmpKey = cmpKey;
    map->head = NULL;
    map->iterator = NULL;
    return map;
}

MapResult mapRemove(Map map, MapKeyElement key_element) {
    Node iter;
    Node prev = EMPTY;
    if (map == NULL || key_element == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    iter = map->head;
    while (iter != END_OF_LIST) {
        if (map->cmpKey(iter->key, key_element) == EQUAL) {
            if (prev == EMPTY_THEN_NODE_IS_HEAD) {
                map->head = map->head->next;
                destroyNode(map, iter);
                return MAP_SUCCESS;
            }
            if (iter->next == NODE_IS_TAIL) {
                prev->next = NULL;
                destroyNode(map, iter);
            } else {
                prev->next = iter->next;
                destroyNode(map, iter);
            }
            return MAP_SUCCESS;
        }
        prev = iter;
        iter = iter->next;
    }
    return MAP_ITEM_DOES_NOT_EXIST;
}

MapResult mapClear(Map map) {
    Node prev;
    map->iterator = NULL;
    if (map == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    prev = map->head;
    while (map->head != EMPTY) {
        map->head = map->head->next;
        destroyNode(map, prev);
        prev = map->head;
    }
    assert(map->head == NULL);
    return MAP_SUCCESS;
}

void mapDestroy(Map map) {
    if (map == NULL) {
        return;
    }
    mapClear(map);
    free(map);
}

int mapGetSize(Map map) {
    int count = 0;
    Node iterator;
    if (map == NULL) {
        return NEG;
    }
    iterator = map->head;
    while (iterator != NULL) {
        count++;
        iterator = iterator->next;
    }
    return count;
}

bool mapContains(Map map, MapKeyElement element) {
    Node iter;
    if (map == NULL || element == NULL || map->head == NULL) {
        return false;
    }
    iter = map->head;
    while (iter != END_OF_LIST) {
        if (map->cmpKey(iter->key, element) == EQUAL) {
            return true;
        }
        iter = iter->next;
    }
    return false;
}

Map mapCopy(Map map) {
    Map new_map;
    Node iter;
    Node new_node;
    Node prev;
    if (map == NULL) {
        return NULL;
    }
    map->iterator = NULL;

    new_map = mapCreate(map->copyData, map->copyKey, map->freeData, map->freeKey, map->cmpKey);

    if (new_map == NULL) {
        return NULL;
    }
    iter = map->head;
    prev = EMPTY;
    new_node = EMPTY;
    while (iter != END_OF_LIST) {
        new_node = nodeCreate(new_map, iter->key, iter->data);
        if (new_node == NULL) {
            mapDestroy(new_map);
            return NULL;
        }
        if (prev == EMPTY_THEN_NODE_IS_HEAD) {
            new_map->head = new_node;
        } else {
            iter->next = new_node;
        }
        prev = iter;
        iter = iter->next;
    }
    return new_map;

}

MapResult mapPut(Map map, MapKeyElement key_element, MapDataElement data_element) {
    Node new_node;
    if (map == NULL || data_element == NULL || key_element == NULL) {
        return MAP_NULL_ARGUMENT;
    }
    map->iterator = NULL;

    if (mapContains(map, key_element)) {
        mapRemove(map, key_element);
    }

    new_node = nodeCreate(map, key_element, data_element);
    if (new_node == NULL) {
       /* free(new_node);*/
        return MAP_OUT_OF_MEMORY;
    }
    insert(map, &new_node);
    return MAP_SUCCESS;
}

MapKeyElement mapGetFirst(Map map) {
    if (map == NULL || map->head == NULL) {
        return NULL;
    }

    map->iterator = map->head;
    return map->copyKey(map->iterator->key);
}

MapKeyElement mapGetNext(Map map) {
    if (map == NULL || map->iterator == NULL) {
        return NULL;
    }

    map->iterator = map->iterator->next;
    if (map->iterator == NULL) {
        return NULL;
    }

    return map->copyKey(map->iterator->key);
}

MapDataElement mapGet(Map map, MapKeyElement key_element) {
    Node iter;
    if (map == NULL || key_element == NULL || map->head == NULL) {
        return NULL;
    }
    iter = map->head;
    while (iter != NULL) {
        if (map->cmpKey(key_element, iter->key) == EQUAL) {
            return iter->data;
        }
        iter = iter->next;
    }

    return NULL;
}
