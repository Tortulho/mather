#include "dict.h"
#include <stdlib.h>
#include <string.h>

Dictionary dictCreate() {

    Dictionary dict;
    dict.entries = malloc(DEFAULT_DICTALLOCMEMSIZE * sizeof(DictEntry));
    if (dict.entries == NULL) {
        return EMPTY_DICT;
    }

    memset(dict.entries,0,DEFAULT_DICTALLOCMEMSIZE * sizeof(DictEntry));
    dict.capacity = DEFAULT_DICTALLOCMEMSIZE;
    dict.size = 0;

    return dict;
}

void dictDestroy(Dictionary *dict) {
    if (dict == NULL) return;
    if (dict->entries == NULL)   return;
    if (dict->capacity == 0)     return;
    //if (dict->size == 0)         return;

    for (size_t i = 0; i < dict->capacity; i++) {
        free(dict->entries[i].key);
    }

    free(dict->entries);
    dict->entries = NULL;
    dict->capacity = 0;
    dict->size = 0;
}

void* dictGet(const Dictionary *dict, const char *key) {

    if (key == NULL || dict == NULL) return 0;
    if (dict->size == 0) return NULL;

    char *dkey;
    for (size_t idx = 0; idx < dict->capacity; idx++) {
        dkey = dict->entries[idx].key;
        if (dkey == NULL) continue;

        if (strcmp(key,dkey) == 0) return dict->entries[idx].value;
    }
    return NULL;

}

int dictIsEmpty(const Dictionary *dict) {
    if (dict == NULL) return 0;
    if (dict->size == 0)
        return 1;
    return 0;
}

void* dictRemove(Dictionary *dict, const char *key) {
    if (key == NULL || dict == NULL) return 0;
    if (dict->size == 0) return NULL;

    char *dkey;
    for (size_t idx = 0; idx < dict->capacity; idx++) {
        dkey = (dict->entries[idx]).key;
        if (dkey == NULL) continue;

        if (strcmp(key,dkey) == 0) {
            free(dkey);
            dict->entries[idx].key = NULL;
            void *pval = (dict->entries[idx]).value;
            dict->entries[idx].value = NULL;
            dict->size--;

            return pval;
        }
    }
    return NULL;
}

int dictInsert(Dictionary *dict, const char *key, void *value) {

    //if (dictIsEmpty(dict)) return 0;
    if (key == NULL || dict == NULL) return 0;
    if (dictExist(dict,key,NULL)) return 0;

    char *dkey;
    for (size_t idx = 0; idx < dict->capacity; idx++) {
        dkey = dict->entries[idx].key;
        if (dkey == NULL) {
            dkey = calloc(strlen(key)+1,1);
            if (dkey == NULL) return 0;
            strcpy(dkey,key);
            (dict->entries[idx]).value = value;
            dict->entries[idx].key = dkey;
            dict->size++;
            return 1;
        }
    }
    //NEED MORE CAP
    DictEntry *temppointer = realloc(dict->entries,dict->capacity*2*sizeof(DictEntry));
    if (temppointer == NULL) return 0;

    size_t oldCapacity = dict->capacity;
    dict->capacity *= 2;
    dict->entries = temppointer;
    memset(dict->entries + oldCapacity,
       0,
       oldCapacity * sizeof(DictEntry));

    dkey = calloc(strlen(key)+1,1);
    if (dkey == NULL) return 0;
    strcpy(dkey,key);

    dict->entries[dict->size].value = value;
    dict->entries[dict->size].key = dkey;
    dict->size++;
    
    return 1;
}

int dictExist(const Dictionary *dict, const char *key, void **getValue) {
    
    if (key == NULL || dict == NULL) return 0;
    void *pointer = dictGet(dict,key);
    if (pointer == NULL) {
        if (getValue != NULL) *getValue = pointer;
        return 0;
    }
    if (getValue != NULL) *getValue = pointer;
    return 1;
    
}

size_t dictSize(const Dictionary *dict) {
    if (dict == NULL) return (size_t)(-1);
    return dict->size;
}