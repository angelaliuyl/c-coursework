#define COLLTYPE "Realloc"

#define FIXEDSIZE 17
#define ROUGHSCALE 2
#define FILMAX 0.6

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
