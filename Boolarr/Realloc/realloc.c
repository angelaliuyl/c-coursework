#include "specific.h"
#include "../boolarr.h"

#define DOTFILE 5000
#define BYTETOBIT 8
#define POWER2 {128, 64, 32, 16, 8, 4, 2, 1}

void _whichbitbyte(int n, unsigned int *whichbit, unsigned int *whichbyte);
bool _and(bool b1, bool b2);
bool _or(bool b1, bool b2);
bool _xor(bool b1, bool b2);

/* All elements initialised to "0" */
boolarr* boolarr_init(void)
{
    boolarr *ba = (boolarr*) ncalloc(sizeof(boolarr), 1);

    ba->a = (datatype*) ncalloc(sizeof(datatype), FIXEDSIZE);
    ba->nbytes = FIXEDSIZE * sizeof(datatype);

    return ba;
}

/* Create boolarr based on string e.g. "1100001" */
boolarr* boolarr_initstr(const char* str)
{
    unsigned int slen = strlen(str);
    int i, j = 0;
    boolarr *ba = (boolarr*) ncalloc(sizeof(boolarr), 1);

    ba->a = (datatype*) ncalloc(sizeof(datatype), FIXEDSIZE);
    ba->nbytes = FIXEDSIZE * sizeof(datatype);
    ba->nbits = slen;
    for (i = slen - 1; i >= 0; i--) {
        switch(str[i]) {
            case '0':
                boolarr_set(ba, j, false);
                break;
            case '1':
                boolarr_set(ba, j, true);
                break;
            default:
                return NULL;
        }
        j++;
    }

    return ba;

}

/* Return a deep copy */
boolarr* boolarr_clone(const boolarr* ba)
{
    boolarr *clone = boolarr_init();
    unsigned int i;
    bool b;
    if (ba == NULL) {
        return NULL;
    }

    for (i = 0; i < ba->nbits; i++) {
        if (boolarr_get(ba, i, &b)) {
            boolarr_set(clone, i, b);
            /*
            if(boolarr_set(clone, i, b)) {
                printf("Successfully set %dth bit of %d\n", i+1, ba->nbits);
            }
            else {
                printf("Unsuccessful in setting %dth bit of %d\n", i+1, ba->nbits);
            }
            */
        }
    }

    return clone;
}

/* Get number of bits in array */
unsigned int boolarr_size(const boolarr* ba)
{
    if (ba == NULL) {
        return 0;
    }
    return ba->nbits;
}

/* Return number of bits that are set true */
unsigned int boolarr_count1s(const boolarr* ba)
{
    unsigned int i, count = 0;
    bool b;
    if (ba==NULL) {
        return 0;
    }

    for (i = 0; i < ba->nbits; i++) {
        if(boolarr_get(ba, i, &b)) {
            if(b) {
                count++;
            }
        }
    }
    /* printf("count = %d\n", count); */

    return count;
}

/* Set nth bit on/off */
bool boolarr_set(boolarr* ba, const unsigned int n, const bool b)
{
    unsigned int whichbit, whichbyte;
    datatype mask, res;

    if (ba==NULL) {
        return false;
    }
    _whichbitbyte(n, &whichbit, &whichbyte);
    /*
    printf("n = %d, whichbit = %d, whichbyte = %d\n", n, whichbit, whichbyte);
    printf("nbits = %d, nbytes = %d\n", ba->nbits, ba->nbytes);
    */
    if (n >= ba->nbits) {
        ba->nbits = n+1;
    }
    while (whichbyte >= ba->nbytes) {
        ba->a = (datatype*) nrecalloc(ba->a, ba->nbytes, sizeof(datatype)*ba->nbytes*SCALEFACTOR);
        ba->nbytes *= SCALEFACTOR;
        /* printf("Whichbyte = %d, Recalloced, size now %d bytes\n", whichbyte, ba->nbytes); */
    }
    mask = 1<<whichbit;
    if(b) {
        res = ba->a[whichbyte] | mask;
    }
    else {
        res = ba->a[whichbyte] & ~mask;
    }
    ba->a[whichbyte] = res;
    return true;

}
/* Get nth bit */
bool boolarr_get(const boolarr* ba, const unsigned int n, bool* b)
{
    unsigned int whichbit, whichbyte;
    datatype mask, res;
    if ((ba==NULL) || (n >= ba->nbits)) {
        return false;
    }

    _whichbitbyte(n, &whichbit, &whichbyte);
    mask = 1<<whichbit;
    res = ba->a[whichbyte] & mask;
    if(res) {
        *b = true;
    }
    else {
        *b = false;
    }
    return true;
}

/* Return if two arrays are the same (bitwise) */
bool boolarr_issame(const boolarr* b1, const boolarr* b2)
{
    unsigned int i;
    bool ba, bb;

    if ((b1 == NULL) != (b2 == NULL)) {
        /* printf("One is null\n"); */
        return false;
    }
    if (b1 == NULL && b2 == NULL) {
        /* printf("Both are null\n"); */
        return true;
    }
    if (b1->nbits != b2->nbits) {
        /* printf("size 1 = %d, size 2 = %d - not equal\n", b1->nbits, b2->nbits); */
        return false;
    }
    for (i = 0; i < b1->nbits; i++) {
        if ((boolarr_get(b1, i, &ba)) && (boolarr_get(b2, i, &bb)) && (ba != bb)) {
            /* printf("For %dth bit, not equal\n", i+1); */
            return false;
        }
    }

    return true;

}

/* Store to string - rightmost bit is LSB */
bool boolarr_tostring(const boolarr* ba, char* str)
{
    unsigned int i, j;
    bool b;
    if (ba == NULL || str == NULL) {
        return false;
    }
    j = ba->nbits;
    str[j] = '\0';
    j--;
    for (i = 0; i < ba->nbits; i++) {
        boolarr_get(ba, i, &b);
        if(b) {
            str[j] = '1';
        }
        else {
            str[j] = '0';
        }
        j--;
    }
    return true;
}

/* Print out array & meta info */
bool boolarr_print(const boolarr* ba)
{
    char *str;
    if (ba == NULL) {
        return false;
    }
    str = ncalloc(ba->nbits + 1, sizeof(char));
    printf("size = %d bits, capacity = %d bytes\n", ba->nbits, ba->nbytes);
    boolarr_tostring(ba, str);
    printf("array = %s\n", str);
    free(str);
    return true;
}

/* Flip all bits */
bool boolarr_negate(boolarr* ba)
{
    unsigned int i;
    bool b;
    if (ba == NULL) {
        return false;
    }

    for (i = 0; i < ba->nbits; i++) {
        boolarr_get(ba, i, &b);
        if(b) {
            boolarr_set(ba, i, false);
        }
        else {
            boolarr_set(ba, i, true);
        }
    }

    return true;
}

/* Functions dealing with 2 bitwise-arrays */
/* Must be the same length */
boolarr* boolarr_bitwise(const boolarr* ba1, const boolarr* ba2, const logicalop l)
{
    boolarr *bc;
    unsigned int i;
    bool b1, b2, res;
    bool(*f[])(bool, bool) = {_or, _and, _xor};

    if (ba1 == NULL || ba2 == NULL) {
        /* printf("at least one of inputs is NULL\n"); */
        return NULL;
    }
    if (ba1->nbits != ba2->nbits) {
        /* printf("number of bits are not equal\n"); */
        return NULL;
    }

    bc = boolarr_init();

    for (i = 0; i < ba1->nbits; i++) {
        if (!boolarr_get(ba1, i, &b1) || !boolarr_get(ba2, i, &b2)) {
            /* printf("something went wrong with get %d\n", i); */
            boolarr_free(bc);
            return NULL;
        }
        res = f[l](b1, b2);
        boolarr_set(bc, i, res);
    }

    return bc;

}

/* Clears all space */
bool boolarr_free(boolarr* p) {
    if (p == NULL) {
        return false;
    }
    free(p->a);
    free(p);
    return true;
}

void _whichbitbyte(int n, unsigned int *whichbit, unsigned int *whichbyte)
{
    *whichbit = n % BYTETOBIT;
    *whichbyte = n / BYTETOBIT;
}

bool _and(bool b1, bool b2)
{
    return (b1 & b2);
}

bool _or(bool b1, bool b2)
{
    return (b1 | b2);
}

bool _xor(bool b1, bool b2)
{
    return (b1 ^ b2);
}
