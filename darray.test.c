#include <stdio.h>
#include <string.h>
#include "darray.h"

darray *someFunction(int i) {
  darray *vec = darrayNew();
  char str[10];
  sprintf(str, "%d", i);
  darrayPush(vec, strdup(str));
  sprintf(str, "%d", i+1);
  darrayPush(vec, strdup(str));
  sprintf(str, "%d", i+2);
  darrayPush(vec, strdup(str));
  return vec;
}

int main() {
  printf("Initializing darray...\n");
  darray *vec = darrayNew();
  darrayPrint("%d", vec);
  puts("");

  printf("Pushing darray...\n");
  for (int i=0; i<100000; i++) {
    printf("Pushing %d\n", i);
    darrayPush(vec, (void *)i);
  }
  darrayPrint("%d", vec);
  printf("Size : %d\n", vec->size);
  puts("");

  printf("Inserting 10 in 3...\n");
  darrayInsert(vec, 3, (void *)10);
  darrayPrint("%d", vec);
  puts("");

  printf("Printf darray at 3 and 4: %d, %d\n", darrayAt(vec, 3), darrayAt(vec, 4));
  puts("");

  printf("Popping darray\n");
  printf("Popped: %d\n", darrayPop(vec));
  darrayPrint("%d", vec);
  puts("");

  printf("Deleting index 3\n");
  darrayDelete(vec, 3);
  darrayPrint("%d", vec);
  puts("");

  printf("Finding 10 in darray: %d\n", darrayFind(vec, (void *)10));
  printf("Finding 5 in darray: %d\n", darrayFind(vec, (void *)5));
  darrayPrint("%d", vec);
  puts("");

  printf("Test string darray\n");
  darray *vec2 = darrayNew();
  for (int i=0; i<10; i++) {
    printf("Pushing %d\n", i);
    darrayPush(vec2, (void *)strdup("Hello Bang"));
  }
  darrayPrint("%s", vec2);

  darray *vec3 = darrayNew();
  darrayPush(vec3, someFunction(1));
  darrayPush(vec3, someFunction(2));
  darrayPush(vec3, someFunction(3));
  for (int i=0; i<vec3->size; i++) {
    darrayPrint("%s", darrayAt(vec3, i));
  }

  
  return 0;
}
