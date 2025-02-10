#include "darray.h"
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE (3 * sizeof(size_t))
#define GET_META(darray) ((size_t*)(darray) - 3)
#define GET_COUNT(darray) (GET_META(darray)[0])
#define GET_CAPACITY(darray) (GET_META(darray)[1])
#define GET_ITEM_SIZE(darray) (GET_META(darray)[2])

void* darray_create(size_t itemSize) {
    size_t* meta = malloc(HEADER_SIZE);
    if (!meta) return NULL;
    meta[0] = 0; // count
    meta[1] = 4; // initial capacity
    meta[2] = itemSize; // item size
    void* array = malloc(itemSize * meta[1]);
    if (!array) {
        free(meta);
        return NULL;
    }
    return (void*)(meta + 3);
}

void darray_destroy(void* darray, void(*destroy)(void *)) {
    if (!darray) return;
    size_t* meta = GET_META(darray);
    if (destroy) {
        for (size_t i = 0; i < meta[0]; i++) {
            destroy((char*)darray + i * meta[2]);
        }
    }
    free(meta);
}

void* darray_init(void* darray, size_t itemSize, void(*destroy)(void *)) {
    darray_destroy(darray, destroy);
    return darray_create(itemSize);
}

void darray_clear(void* darray, void(*destroy)(void *)) {
    if (!darray) return;
    size_t* meta = GET_META(darray);
    if (destroy) {
        for (size_t i = 0; i < meta[0]; i++) {
            destroy((char*)darray + i * meta[2]);
        }
    }
    meta[0] = 0;
}

size_t darray_count(const void* darray) {
    return darray ? GET_COUNT(darray) : INVALID;
}

void* darray_item(void* darray, size_t i) {
    size_t* meta = GET_META(darray);
    return (i < meta[0]) ? (char*)darray + i * meta[2] : NULL;
}

void* darray_add(void* darray) {
    size_t* meta = GET_META(darray);
    if (meta[0] == meta[1]) { // Expand capacity
        meta[1] *= 2;
        size_t* newMeta = realloc(meta, HEADER_SIZE + meta[1] * meta[2]);
        if (!newMeta) return NULL;
        meta = newMeta;
        darray = (void*)(meta + 3);
    }
    void* newItem = (char*)darray + meta[0] * meta[2];
    memset(newItem, 0, meta[2]);
    meta[0]++;
    return newItem;
}

void* darray_insert(void* darray, size_t i) {
    size_t* meta = GET_META(darray);
    if (i > meta[0]) return NULL;
    if (meta[0] == meta[1]) { // Expand capacity
        meta[1] *= 2;
        size_t* newMeta = realloc(meta, HEADER_SIZE + meta[1] * meta[2]);
        if (!newMeta) return NULL;
        meta = newMeta;
        darray = (void*)(meta + 3);
    }
    memmove((char*)darray + (i + 1) * meta[2], (char*)darray + i * meta[2], (meta[0] - i) * meta[2]);
    memset((char*)darray + i * meta[2], 0, meta[2]);
    meta[0]++;
    return (char*)darray + i * meta[2];
}

void darray_remove(void* darray, size_t i, void(*destroy)(void *)) {
    size_t* meta = GET_META(darray);
    if (i >= meta[0]) return;
    void* item = (char*)darray + i * meta[2];
    if (destroy) destroy(item);
    memmove(item, (char*)item + meta[2], (meta[0] - i - 1) * meta[2]);
    meta[0]--;
}

size_t darray_first(const void* darray) { return darray && GET_COUNT(darray) > 0 ? 0 : INVALID; }
size_t darray_last(const void* darray) { return darray && GET_COUNT(darray) > 0 ? GET_COUNT(darray) - 1 : INVALID; }
size_t darray_next(const void* darray, size_t item_id) { return (darray && item_id + 1 < GET_COUNT(darray)) ? item_id + 1 : INVALID; }
size_t darray_prev(const void* darray, size_t item_id) { return (darray && item_id > 0) ? item_id - 1 : INVALID; }
size_t darray_stop(const void* darray) { return INVALID; }
void* darray_current(const void* darray, size_t item_id) { return darray_item((void*)darray, item_id); }

void darray_erase(void* darray, size_t item_id, void(*destroy)(void *)) {
    darray_remove(darray, item_id, destroy);
}
