#include <stdlib.h>
#include <string.h>
#include "reverseIndex.h"

reverseIndex *reverseIndexNew() {
  reverseIndex *ri = malloc(sizeof(reverseIndex));
  ri->words = darrayNew();
  ri->indexes = darrayNew();
  return ri;
}

void reverseIndexAdd(reverseIndex *ri, char *word, size_t index) {
  int found = 0;

  for (size_t k = 0; k < ri->words->size; k++) {
    char *sword = (char *)ri->words->items[k];
    if (strcmp(word, sword) == 0) {
      darrayPush((darray *)ri->indexes->items[k], (void *)(size_t)index);
      found = 1;
      break;
    }
  }

  if (!found) {
    darrayPush(ri->words, strndup(word, strlen(word)));
    darray *newIndexArray = darrayNew();
    darrayPush(newIndexArray, (void *)(size_t)index);
    darrayPush(ri->indexes, newIndexArray);
  }
};

void reverseIndexPrint(reverseIndex *ri){
  for (size_t i = 0; i < ri->words->size; i++) {
    char *word = (char *)ri->words->items[i];
    darray *indexArray = (darray *)ri->indexes->items[i];
    printf("%s: ", word);
    darrayPrint("%d", indexArray);
  }
};
