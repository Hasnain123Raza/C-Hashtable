#include <stdio.h>
#include <string.h>

#include "hashtable.h"

static void hashtablePrint(Hashtable *hashtable);
static int hasher(void *value);
static int comparator(void *first, void *second);



int main()
{
    Hashtable *hashtable = hashtableCreate(4, hasher, comparator);

    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "Hello");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "World");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "How");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "Are");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "You");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "Today");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "What");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "Is");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "Up");
    hashtablePrint(hashtable);
    hashtableAdd(hashtable, "With");
    hashtablePrint(hashtable);

    hashtableRemove(hashtable, "Hello");
    hashtablePrint(hashtable);
    hashtableRemove(hashtable, "World");
    hashtablePrint(hashtable);
    hashtableRemove(hashtable, "How");
    hashtablePrint(hashtable);
    hashtableRemove(hashtable, "Are");
    hashtablePrint(hashtable);

    hashtableFree(hashtable, NULL);

    return 0;
}



static void hashtablePrint(Hashtable *hashtable)
{
    for (int counter = 0; counter < hashtable->capacity; counter++)
    {
        printf("%d: ", counter);

        Bucket *bucket = hashtable->buckets[counter];
        while (bucket != NULL)
        {
            printf("%s -> ", (char *)bucket->value);
            bucket = bucket->next;
        }

        printf("NULL\n");
    }

    printf("\n");
}

static int hasher(void *value)
{
    char *string = (char *)value;

    unsigned long hash = 5381;
    int character;

    while ((character = *string++))
        hash = ((hash << 5) + hash) + character;

    return hash;
}

static int comparator(void *first, void *second)
{
    return strcmp((char *)first, (char *)second);
}