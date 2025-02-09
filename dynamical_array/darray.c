#include "darray.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Структура динамического массива
typedef struct {
    void *data;       // Данные
    size_t itemSize;  // Размер элемента
    size_t count;     // Количество элементов
    size_t capacity;  // Ёмкость массива
} DArray;

// Внутренний метод для увеличения ёмкости массива
static bool darray_resize(DArray *darray, size_t new_capacity) {
    void *new_data = realloc(darray->data, new_capacity * darray->itemSize);
    if (!new_data) return false;
    darray->data = new_data;
    darray->capacity = new_capacity;
    return true;
}

void *darray_create(size_t itemSize) {
    DArray *darray = (DArray *)malloc(sizeof(DArray));
    if (!darray) return NULL;
    darray->data = NULL;
    darray->itemSize = itemSize;
    darray->count = 0;
    darray->capacity = 0;
    return darray;
}

void darray_destroy(void *darray, void (*destroy)(void *)) {
    DArray *arr = (DArray *)darray;
    if (!arr) return;
    if (destroy) {
        for (size_t i = 0; i < arr->count; i++) {
            destroy((char *)arr->data + i * arr->itemSize);
        }
    }
    free(arr->data);
    free(arr);
}

void *darray_init(void *darray, size_t itemSize, void (*destroy)(void *)) {
    darray_destroy(darray, destroy);
    return darray_create(itemSize);
}

void darray_clear(void *darray, void (*destroy)(void *)) {
    DArray *arr = (DArray *)darray;
    if (!arr) return;
    if (destroy) {
        for (size_t i = 0; i < arr->count; i++) {
            destroy((char *)arr->data + i * arr->itemSize);
        }
    }
    arr->count = 0;
}

size_t darray_count(const void *darray) {
    return darray ? ((DArray *)darray)->count : INVALID;
}

void *darray_item(void *darray, size_t i) {
    DArray *arr = (DArray *)darray;
    if (!arr || i >= arr->count) return NULL;
    return (char *)arr->data + i * arr->itemSize;
}

void *darray_add(void *darray) {
    DArray *arr = (DArray *)darray;
    if (!arr) return NULL;
    if (arr->count == arr->capacity) {
        if (!darray_resize(arr, arr->capacity ? arr->capacity * 2 : 4)) return NULL;
    }
    void *newItem = (char *)arr->data + arr->count * arr->itemSize;
    arr->count++;
    return newItem;
}

void *darray_insert(void *darray, size_t i) {
    DArray *arr = (DArray *)darray;
    if (!arr || i > arr->count) return NULL;
    if (arr->count == arr->capacity) {
        if (!darray_resize(arr, arr->capacity ? arr->capacity * 2 : 4)) return NULL;
    }
    memmove((char *)arr->data + (i + 1) * arr->itemSize,
            (char *)arr->data + i * arr->itemSize,
            (arr->count - i) * arr->itemSize);
    arr->count++;
    return (char *)arr->data + i * arr->itemSize;
}

void darray_remove(void *darray, size_t i, void (*destroy)(void *)) {
    DArray *arr = (DArray *)darray;
    if (!arr || i >= arr->count) return;
    void *item = (char *)arr->data + i * arr->itemSize;
    if (destroy) destroy(item);
    memmove(item, (char *)item + arr->itemSize, (arr->count - i - 1) * arr->itemSize);
    arr->count--;
}

size_t darray_first(const void *darray) { return darray_count(darray) > 0 ? 0 : INVALID; }
size_t darray_last(const void *darray) { return darray_count(darray) > 0 ? ((DArray *)darray)->count - 1 : INVALID; }
size_t darray_next(const void *darray, size_t item_id) { return item_id + 1 < darray_count(darray) ? item_id + 1 : INVALID; }
size_t darray_prev(const void *darray, size_t item_id) { return item_id > 0 ? item_id - 1 : INVALID; }
size_t darray_stop(const void *darray) { return INVALID; }
void *darray_current(const void *darray, size_t item_id) { return darray_item((void *)darray, item_id); }

void darray_erase(void *darray, size_t item_id, void (*destroy)(void *)) {
    darray_remove(darray, item_id, destroy);
}
