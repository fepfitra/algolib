#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  void **items;
  unsigned int size;
  unsigned int capacity;
} darray;

const unsigned int DARRAY_INIT_CAPACITY = 0x10;

void darrayInit(darray *vec) {
  vec->capacity = DARRAY_INIT_CAPACITY;
  vec->size = 0;
  vec->items = malloc(sizeof(void *) * vec->capacity);
}

void *darrayAt(darray *vec, unsigned int index) {
  assert(index < vec->size);
  return *(vec->items + sizeof(void *) * index);
}

void darrayResize(darray *vec, int new_size) {
  assert(new_size > 0);
  vec->capacity = new_size;

  void **new_items = realloc(vec->items, sizeof(void *) * vec->capacity);

  if (new_items == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  vec->items = new_items;
}

void darrayPush(darray *vec, void *item) {
  if (vec->size == vec->capacity) {
    darrayResize(vec, vec->capacity * 1.5);
  }
  vec->items[vec->size] = item;
  vec->size++;
}

void darrayInsert(darray *vec, int index, void *item) {
  assert(index >= 0 && index <= vec->size);

  if (vec->size == vec->capacity) {
    int new_capacity = vec->capacity > 0 ? vec->capacity * 1.5 : 1;
    darrayResize(vec, new_capacity);
  }

  for (int i = vec->size - 1; i >= index; i--) {
    vec->items[i + 1] = vec->items[i];
  }

  vec->items[index] = item;

  vec->size++;
}

void *darrayPop(darray *vec) {
  assert(vec->size > 0);
  if (vec->size <= vec->capacity / 4) {
    darrayResize(vec, vec->capacity / 2);
  }
  void *ret_val = vec->items[vec->size - 1];

  vec->items[vec->size - 1] = NULL;
  vec->size--;
  return ret_val;
}

void darrayDelete(darray *vec, int index) {
  assert(index > 0 && index < vec->size);
  if (vec->size <= vec->capacity / 4) {
    // half the capacity in resize
    darrayResize(vec, vec->capacity / 2);
  }
  vec->items[index] = NULL;
  for (int i = index; i < vec->size; i++) {
    vec->items[i] = vec->items[i + 1];
  }
  vec->size--;
}

int darrayFind(darray *vec, void *item) {
  for (int i = 0; i < vec->size; i++) {
    // if (*(vec->items + sizeof(void *) * i) == item) {
    //   return i;
    // }
    if (vec->items[i] == item) {
      return i;
    }
  }
  return -1;
}

// void darrayRemove(darray *vec, void *item) {
//   for (int i = 0; i < vec->size; i++) {
//     if (*(vec->items + sizeof(void *) * i) == item) {
//       darrayDelete(vec, i);
//     }
//   }
// }

void darrayPrint(char *format, darray *vec) {
  printf("[");
  for (int i = 0; i < vec->size; i++) {
    printf(format, vec->items[i]);
    if (i < vec->size - 1) {
      printf(", ");
    }
  }
  printf("]\n");
}

void darrayDestroy(darray *vec) {
  free(vec->items);
  vec->items = NULL;
  vec->size = 0;
  vec->capacity = 0x0;
}
