#include "darray.h"

#include <stdlib.h>


typedef struct {
    void *data;
    size_t itemSize;
    size_t count;
    size_t capacity;
} DArray;


void* darray_create(size_t itemSize){
  DArray* darray = (DArray*)malloc(sizeof(DArray));
  if (!darray){
    return NULL;
  }
  darray->data = NULL;
  darray->itemSize = itemSize;
  darray->count = 0;
  darray->capacity = 0;
  return darray;
}

void darray_destroy(void * darray, void(*destroy)( void * )){
  DArray *tmp = (DArray *)darray;
    if (!tmp) {
      return;
    }
    if (destroy) {
        for (size_t i = 0; i < tmp->count; i++) {
            destroy((char *)tmp->data + i * tmp->itemSize);
        }
    }
    free(tmp->data);
    tmp->data = NULL;
    free(tmp);
    tmp = NULL;
}

void *darray_init(void *darray, size_t itemSize, void (*destroy)(void *)) {
    darray_destroy(darray, destroy);
    return darray_create(itemSize);
}

void darray_clear(void *darray, void (*destroy)(void *)) {
    DArray *tmp = (DArray *)darray;
    if (!tmp) {
      return;
    }
    if (destroy) {
        for (size_t i = 0; i < tmp->count; i++) {
            destroy((char *)tmp->data + i * tmp->itemSize);
        }
    }
    tmp->count = 0;
}