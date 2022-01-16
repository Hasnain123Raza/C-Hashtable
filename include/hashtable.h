#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>

typedef struct Bucket {
    void *value;
    struct Bucket *next;
} Bucket;

typedef int (*Hasher)(void *);
typedef int (*Comparator)(void *, void *);

typedef struct {
    int minimumCapacity;
    int capacity;
    int length;
    Bucket **buckets;
    Hasher hasher;
    Comparator comparator;
} Hashtable;

Hashtable *hashtableCreate(int minimumCapacity, Hasher hasher, Comparator comparator);
int hashtableResize(Hashtable *hashtable, int newCapacity);
void hashtableFree(Hashtable *hashtable);

int hashtableAdd(Hashtable *hashtable, void *value);
void *hashtableRemove(Hashtable *hashtable, void *value);
void *hashtableGet(Hashtable *hashtable, void *value);

#endif