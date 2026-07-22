#include <stdlib.h>
#include "dynarr.h"
#include <string.h>

dynarr dynarrCreate() {
    dynarr arr;
    arr.data = malloc(DEFAULT_DYNARRAY_CAPACITY * sizeof(void*));
    if (arr.data == NULL) return EMPTY_DYNARR;
    memset(arr.data, 0, DEFAULT_DYNARRAY_CAPACITY * sizeof(void *));
    arr.capacity = DEFAULT_DYNARRAY_CAPACITY;
    arr.size = 0;
    return arr;
}

void dynarrDestroy(dynarr *array) {
    if (array == NULL) return;
    if (array->data == NULL) return;
    free(array->data);
    array->data = NULL;
    array->capacity = 0;
    array->size = 0;
}

int dynarrPush(dynarr *array, void *value) {
    if (array == NULL) return 0;

    if (array->size == array->capacity) {

        size_t newCapacity = array->capacity * 2;

        void **temp = realloc(array->data, newCapacity * sizeof(void *));

        if (temp == NULL)
            return 0;

        memset(temp + array->capacity, 0,
               (newCapacity - array->capacity) * sizeof(void *));

        array->data = temp;
        array->capacity = newCapacity;
    }

    array->data[array->size] = value;
    array->size++;

    return 1;
}

void *dynarrGet(const dynarr *array, size_t index) {
    if (array == NULL) return NULL;
    if (index >= array->size) return NULL;
    return array->data[index];
}

int dynarrSet(dynarr *array, size_t index, void *value) {
    if (array == NULL) return 0;

    if (index >= array->size) return 0;

    array->data[index] = value;

    return 1;
}

void *dynarrPop(dynarr *array) {
    if (array == NULL) return NULL;
    if (array->size == 0) return NULL;
    void *returnp = array->data[array->size-1];
    array->data[array->size-1] = NULL;
    array->size--;
    return returnp;
}

size_t dynarrSize(const dynarr *array) {
    if (array == NULL) return 0;
    return array->size;
}

int dynarrIsEmpty(const dynarr *array) {
    if (array == NULL) return 1;
    return (array->size == 0);
}