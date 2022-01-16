#include "hashtable.h"

Hashtable *hashtableCreate(int minimumCapacity, Hasher hasher, Comparator comparator)
{
    if (minimumCapacity < 1 || !hasher || !comparator)
        return NULL;

    Hashtable *hashtable = (Hashtable *)malloc(sizeof(Hashtable));
    if (hashtable == NULL)
        return NULL;

    Bucket **buckets = (Bucket **)malloc(sizeof(Bucket *) * minimumCapacity);
    if (buckets == NULL)
    {
        free(hashtable);
        return NULL;
    }
    memset(buckets, 0, sizeof(Bucket *) * minimumCapacity);

    hashtable->minimumCapacity = minimumCapacity;
    hashtable->capacity = minimumCapacity;
    hashtable->length = 0;
    hashtable->buckets = buckets;
    hashtable->hasher = hasher;
    hashtable->comparator = comparator;

    return hashtable;
}

int hashtableResize(Hashtable *hashtable, int newCapacity)
{
    if (newCapacity < hashtable->minimumCapacity)
        return 1;
    
    Bucket **newBuckets = (Bucket **)malloc(sizeof(Bucket *) * newCapacity);
    if (newBuckets == NULL)
        return 1;
    memset(newBuckets, 0, sizeof(Bucket *) * newCapacity);

    for (int counter = 0; counter < hashtable->capacity; counter++)
    {
        Bucket *bucket = hashtable->buckets[counter];
        while (bucket != NULL)
        {
            Bucket *next = bucket->next;
            int index = hashtable->hasher(bucket->value) % newCapacity;
            bucket->next = newBuckets[index];
            newBuckets[index] = bucket;
            bucket = next;
        }
    }

    free(hashtable->buckets);
    hashtable->buckets = newBuckets;
    hashtable->capacity = newCapacity;

    return 0;
}

void hashtableFree(Hashtable *hashtable, Free freeFunction)
{
    if (!hashtable)
        return;

    for (int counter = 0; counter < hashtable->capacity; counter++)
    {
        Bucket *bucket = hashtable->buckets[counter];
        while (bucket)
        {
            Bucket *next = bucket->next;
            if (freeFunction)
                freeFunction(bucket->value);
            free(bucket);
            bucket = next;
        }
    }

    free(hashtable->buckets);
    free(hashtable);
}



int hashtableAdd(Hashtable *hashtable, void *value)
{
    if (!hashtable || !value)
        return 1;

    Bucket *newBucket = (Bucket *)malloc(sizeof(Bucket));
    if (newBucket == NULL)
        return 1;
    newBucket->value = value;
    newBucket->next = NULL;

    int index = hashtable->hasher(value) % hashtable->capacity;
    Bucket *bucket = hashtable->buckets[index];

    if (bucket)
    {
        while (bucket->next)
            bucket = bucket->next;

        bucket->next = newBucket;
    }
    else
    {
        hashtable->buckets[index] = newBucket;
        hashtable->length++;

        float loadFactor = (float)hashtable->length / hashtable->capacity;
        if (loadFactor >= 0.75)
            hashtableResize(hashtable, hashtable->capacity * 2);
    }

    return 0;
}

void *hashtableRemove(Hashtable *hashtable, void *value)
{
    if (!hashtable || !value)
        return NULL;
    
    Bucket *foundBucket = NULL;
    void *foundValue = NULL;

    int index = hashtable->hasher(value) % hashtable->capacity;
    Bucket *bucket = hashtable->buckets[index];

    if (bucket)
    {
        if (hashtable->comparator(bucket->value, value) == 0)
        {
            foundBucket = bucket;
            hashtable->buckets[index] = bucket->next;
            hashtable->length--;

            float loadFactor = (float)hashtable->length / hashtable->capacity;
            if (loadFactor <= 0.25)
                hashtableResize(hashtable, hashtable->capacity / 2);
        }
        else
        {
            while (bucket->next)
            {
                if (hashtable->comparator(bucket->next->value, value) == 0)
                {
                    foundBucket = bucket->next;
                    bucket->next = foundBucket->next;
                    break;
                }
                bucket = bucket->next;
            }
        }
    }

    if (foundBucket)
    {
        foundValue = foundBucket->value;
        free(foundBucket);
    }

    return foundValue;
}

void *hashtableGet(Hashtable *hashtable, void *value)
{
    if (!hashtable || !value)
        return NULL;

    int index = hashtable->hasher(value) % hashtable->capacity;
    Bucket *bucket = hashtable->buckets[index];

    while (bucket)
    {
        if (hashtable->comparator(bucket->value, value) == 0)
            return bucket->value;
        bucket = bucket->next;
    }

    return NULL;
}