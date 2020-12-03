#include "../../General/bool.h"

typedef unsigned char datatype;
#define FORMATSTR "%c"
#define ELEMSIZE 20

#define STACKTYPE "Realloc"

#define FIXEDSIZE 16
#define SCALEFACTOR 2

struct boolarr {
   /* Underlying array */
   datatype* a;
   unsigned int nbytes;
   unsigned int nbits;
};
typedef struct boolarr boolarr;
