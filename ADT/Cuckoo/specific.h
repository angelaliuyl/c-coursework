#define COLLTYPE "Cuckoo"

#define FIXEDSIZE 16
#define SCALEFACTOR 2
#define FILMAX 0.6
#define TABLENUM 2

struct pair {
    void *key;
    void *data;
};
typedef struct pair pair;

struct assoc {
    pair *hashtable;
    int keysize;
    unsigned int size;
    unsigned int capacity;
};
typedef struct assoc assoc;
