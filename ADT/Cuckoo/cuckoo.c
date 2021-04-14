#include "specific.h"
#include "../assoc.h"
#include <math.h>

#define SWAP(a, b, tmp) tmp = a; a = b; b = tmp;

typedef enum bool {false, true} bool;
typedef enum tablenum {first, second} tablenum;

unsigned int _hash1(void *key, unsigned int capacity, int keysize);
unsigned int _hash2(void *key, unsigned int capacity, int keysize);
void _resize(assoc **a);
bool _keymatch(void *key1, void *key2, unsigned int keysize);

assoc* assoc_init(int keysize)
{
    assoc *a;

    if (keysize < 0) {
        /* keysize should not be smaller than 0 */
        return NULL;
    }

    a = ncalloc(1, sizeof(assoc));
    a->hashtable = ncalloc(FIXEDSIZE, sizeof(pair));
    a->keysize = keysize;
    a->size = 0;
    a->capacity = FIXEDSIZE;

    return a;
}

void assoc_insert(assoc** a, void* key, void* data)
{
    unsigned int hash, pushed, pushlimit;
    tablenum table = first;
    float filled;
    pair *current;
    void *tmpkey, *tmpdata;
    assoc* as = *a;

    if (as == NULL || key == NULL) {
        return;
    }

    /* if table filled more than FILMAX, then resize */
    filled = (float) as->size / (float) as->capacity;
    if (filled > FILMAX) {
        _resize(a);
        as = *a;
    }

    /* checking if number is already in hash tables, if so just update data */
    hash = _hash2(key, as->capacity, as->keysize);
    current= &as->hashtable[hash];
    if(current->key != NULL && _keymatch(current->key, key, as->keysize)) {
        current->data = data;
        return;
    }
    hash = _hash1(key, as->capacity, as->keysize);
    current = &as->hashtable[hash];
    if(current->key != NULL && _keymatch(current->key, key, as->keysize)) {
        current->data = data;
        return;
    }

    pushed = 0;
    while (current->key != NULL) {
        /* pushing key and data in current pair out, placing new stuff in */
        SWAP (current->key, key, tmpkey);
        SWAP (current->data, data, tmpdata);
        if (table == first) {
            /* change to second table */
            table = second;
            hash = _hash2(key, as->capacity, as->keysize);
            current = &as->hashtable[hash];
        }
        else if (table == second){
            /* change to first table */
            table = first;
            hash = _hash1(key, as->capacity, as->keysize);
            current = &as->hashtable[hash];
        }
        pushed++;
        /* checking how many times pushing between the two tables has happened */
        /* if push number is larger than log_2(capacity), then resize */
        pushlimit = (int) log2(as->capacity);
        if (pushed > pushlimit) {
            _resize(a);
            as = *a;
            /* reset push counter */
            pushed = 0;
            table = first;
            hash = _hash1(key, as->capacity, as->keysize);
            current = &as->hashtable[hash];
        }
    }
    /* once empty space is found, insert into hash table */
    current->key = key;
    current->data = data;
    as->size++;
}

unsigned int assoc_count(assoc* a)
{
    if(a == NULL) {
        return 0;
    }
    return a->size;
}

void* assoc_lookup(assoc* a, void* key)
{
    unsigned int hash1, hash2;
    pair *table1, *table2;

    if (a == NULL || key == NULL) {
        return NULL;
    }

    hash1 = _hash1(key, a->capacity, a->keysize);
    hash2 = _hash2(key, a->capacity, a->keysize);
    table1 = &a->hashtable[hash1];
    table2 = &a->hashtable[hash2];
    if (table1->key == NULL && table2->key == NULL) {
        return NULL;
    }
    if (table1->key != NULL) {
        if (_keymatch(table1->key, key, a->keysize)) {
            return table1->data;
        }
    }
    if (table2->key != NULL) {
        if (_keymatch(table2->key, key, a->keysize)) {
            return table2->data;
        }
    }
    return NULL;
}

void assoc_free(assoc* a)
{
    if (a != NULL) {
        free(a->hashtable);
        free(a);
    }
}

unsigned int _hash1(void *key, unsigned int capacity, int keysize)
{
    /* based on djb2 taken from http://www.cse.yorku.ca/~oz/hash.html */
    unsigned int n;
    unsigned long hash = 5381;
    int c, i;
    unsigned char *str = (unsigned char *) key;

    if(keysize == 0) {
        keysize = strlen((char *)str);
    }
    for (i = 0; i < keysize; i++) {
        c = str[i];
        hash = ((hash << 5) + hash) + c;
    }
    /* capacity of first table is half of total capacity and fills first half */
    n = (unsigned int) (hash % (capacity / TABLENUM));
    return n;
}

unsigned int _hash2(void *key, unsigned int capacity, int keysize)
{
    /* baed on sdbm taken from http://www.cse.yorku.ca/~oz/hash.html */
    unsigned int n;
    unsigned long hash = 0;
    int c, i;
    unsigned char *str = (unsigned char *) key;

    if(keysize == 0) {
        keysize = strlen((char *)str);
    }
    for (i = 0; i < keysize; i++) {
        c = str[i];
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    /* capacity of second table is half of total capacity and fills second half */
    n = (unsigned int) (hash % (capacity / TABLENUM)) + (capacity / TABLENUM);
    return n;
}

void _resize(assoc **a)
{
    assoc *new, *old = *a;
    unsigned int newbytes, i;
    pair *current;

    new = assoc_init(old->keysize);
    newbytes = old->capacity * SCALEFACTOR * sizeof(pair);
    new->hashtable = nrecalloc(new->hashtable, FIXEDSIZE * sizeof(pair), newbytes);
    new->capacity = newbytes / sizeof(pair);

    /* rehashing content of old hash table to new, bigger hash table */
    /* assoc_insert handles null keys */
    for (i = 0; i < old->capacity; i++) {
        current = &old->hashtable[i];
        assoc_insert(&new, current->key, current->data);
    }
    assoc_free(old);
    *a = new;
}

bool _keymatch(void *key1, void *key2, unsigned int keysize)
{
    /* in case of str type */
    if (keysize == 0) {
        if (strcmp((char *) key1, (char *) key2) == 0) {
            return true;
        }
    }
    /* in case of other types */
    else {
        if (memcmp(key1, key2, keysize) == 0) {
            return true;
        }
    }

    return false;
}

void test(void)
{
    static char str[50]="hello";
    int i, n, n1, n2;
    unsigned int hash_const, hash_check;
    assoc* a;

    hash_const = _hash1(str, 17, 0);

    for (i = 0; i < 100; i++) {
        hash_check = _hash1(str, 17, 0);
        assert(hash_const == hash_check);
    }

    hash_const = _hash2(str, 17, 0);

    for (i = 0; i < 100; i++) {
        hash_check = _hash2(str, 17, 0);
        assert(hash_const == hash_check);
    }

    n1 = 22;
    n2 = 22;

    hash_const = _hash1(&n1, 17, sizeof(int));

    for (i = 0; i < 100; i++) {
        hash_check = _hash1(&n2, 17, sizeof(int));
        assert(hash_const == hash_check);
    }

    hash_const = _hash2(&n1, 17, sizeof(int));

    for (i = 0; i < 100; i++) {
        hash_check = _hash2(&n2, 17, sizeof(int));
        assert(hash_const == hash_check);
    }

    assert(_keymatch(str, str, 0));
    assert(_keymatch(&n1, &n2, sizeof(int)));

    a = assoc_init(0);
    n = 50;
    assoc_insert(&a, str, &n);
    assert(assoc_lookup(a, str) != NULL);

    assoc_free(a);
}
