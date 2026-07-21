#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <string.h>

#define DEFAULT_DICTALLOCMEMSIZE 128 //128 ENTRIES
#define EMPTY_DICT (Dictionary){NULL,0,0}

typedef struct {

    char *key;
    void *value;

} DictEntry;

typedef struct {

    DictEntry *entries;
    size_t size;
    size_t capacity;

} Dictionary;

Dictionary dictCreate();
void dictDestroy(Dictionary *dict);
void* dictGet(const Dictionary *dict, const char *key);
int dictIsEmpty(const Dictionary *dict);
void* dictRemove(Dictionary *dict, const char *key);
int dictInsert(Dictionary *dict, const char *key, void *value);
int dictExist(const Dictionary *dict, const char *key, void **getValue);
size_t dictSize(const Dictionary *dict);

#endif