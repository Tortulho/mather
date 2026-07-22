#ifndef DYNARR_H
#define DYNARR_H

#include <stdlib.h>
#include <string.h>

#define DEFAULT_DYNARRAY_CAPACITY 8

typedef struct {

    void **data;

    size_t size;
    size_t capacity;

} dynarr;

#define EMPTY_DYNARR (dynarr){NULL,0,0}

dynarr dynarrCreate(void);

void dynarrDestroy(dynarr *array);

int dynarrPush(dynarr *array, void *value);

void *dynarrGet(const dynarr *array, size_t index);

int dynarrSet(dynarr *array, size_t index, void *value);

void *dynarrPop(dynarr *array);

size_t dynarrSize(const dynarr *array);

int dynarrIsEmpty(const dynarr *array);

#endif