#include <stdbool.h>

typedef struct {
  void **items;
  unsigned int size;
  unsigned int capacity;
  bool storeHeap;
} darray;

darray *darrayNew();

void darrayPush(darray *vec, void *item);

void darrayInsert(darray *vec, int index, void *item);

void *darrayAt(darray *vec, unsigned int index);

void *darrayPop(darray *vec);

void darrayDelete(darray *vec, int index);

int darrayFind(darray *vec, void *item);

// void darrayRemove(darray *vec, void *item);

void darrayPrint(char *format, darray *vec);

void darrayDestroy(darray *vec);
