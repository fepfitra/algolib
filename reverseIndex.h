#include "darray.h"
#include <stdio.h>

typedef struct reverseIndex {
  darray *words;
  darray *indexes;
} reverseIndex;


reverseIndex *reverseIndexNew();

void reverseIndexAdd(reverseIndex *ri, char *word, size_t index);

void reverseIndexPrint(reverseIndex *ri);
