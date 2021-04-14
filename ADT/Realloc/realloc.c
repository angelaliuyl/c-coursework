#include "specific.h"
#include "../assoc.h"

typedef enum bool {false, true} bool;

unsigned long _djb(void *key, int keysize);
unsigned int _hash1(void *key, unsigned int capacity, int keysize);
unsigned int _hash2(void *key, unsigned int capacity, int keysize);
void _resize(assoc **a);
unsigned int _next_prime(unsigned int n);
bool _is_prime(unsigned int n);
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
    unsigned int hash, probe;
    float filled;
    pair *current;
    assoc* as = *a;

    if (as == NULL || key == NULL) {
        return;
    }

    filled = (float) as->size / (float) as->capacity;
    /* if more than FILMAX, rehash table */
    if (filled > FILMAX) {
        _resize(a);
        as = *a;
    }
    hash = _hash1(key, as->capacity, as->keysize);
    probe = _hash2(key, as->capacity, as->keysize);
    current = &as->hashtable[hash];
    while (current->key != NULL) {
        if(_keymatch(current->key, key, as->keysize)) {
            /* key already in table, updated data */
            current->data = data;
            return;
        }
        hash = (hash + probe) % as->capacity;
        current = &as->hashtable[hash];
    }
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
    unsigned int hash, probe, count = 1;
    pair *current;

    if (a == NULL || key == NULL) {
        return NULL;
    }

    hash = _hash1(key, a->capacity, a->keysize);
    probe = _hash2(key, a->capacity, a->keysize);
    current = &a->hashtable[hash];
    /* if nothing having this hash has been placed in hash table */
    if(current->key == NULL) {
        return NULL;
    }
    /* look through all of hash table or until probes an empty space */
    while (count <= a->size && current->key != NULL) {
        if (_keymatch(current->key, key, a->keysize)) {
            return current->data;
        }
        hash = (hash + probe) % a->capacity;
        current = &a->hashtable[hash];
        count++;
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

unsigned long _djb(void *key, int keysize)
{
    /* based on djb2 from http://www.cse.yorku.ca/~oz/hash.html and modified */
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

    return hash;
}

unsigned int _hash1(void *key, unsigned int capacity, int keysize)
{
    /* combines djb and hash function in data_structs notes pg. 89 */
    unsigned int n;
    n = (unsigned int) (_djb(key, keysize) % capacity);
    return n;
}

unsigned int _hash2(void *key, unsigned int capacity, int keysize)
{
    /* combines djb and hash function in data_structs notes pg. 93 */
    unsigned int hash2 = (unsigned int) (_djb(key, keysize)/capacity) % capacity;

    if (1 > hash2) {
        return 1;
    }
    else {
        return hash2;
    }
}

void _resize(assoc **a)
{
    assoc *new, *old = *a;
    unsigned int newbytes, i;
    pair *current;

    new = assoc_init(old->keysize);
    newbytes = _next_prime(old->capacity * ROUGHSCALE) * sizeof(pair);
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

unsigned int _next_prime(unsigned int n)
{
    while (!_is_prime(n)) {
        n++;
    }
    return n;
}

bool _is_prime(unsigned int n)
{
    unsigned int i;
    /* using definition of prime numbers */
    if (n == 0 || n == 1) {
        return false;
    }
    else if (n == 2) {
        return true;
    }
    else {
        for (i = 2; i < n/2; i++) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }
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
    unsigned long numc, numn;
    int i, n, n1, n2;
    unsigned int hash_const, hash_check;
    assoc* a;

    numc = _djb(str, 0);

    for (i = 0; i < 100; i++) {
        numn = _djb(str, 0);
        assert(numc == numn);
    }

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

    n1 = 3303;
    n2 = 3303;

    assert(_keymatch(&n1, &n2, sizeof(int)));

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
