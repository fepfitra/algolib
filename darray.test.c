#include <stdio.h>
#include "darray.h"

int main() {
  printf("Initializing darray...\n");
  darray vec;
  darrayInit(&vec);
  darrayPrint("%d", &vec);
  puts("");

  printf("Pushing darray...\n");
  darrayPush(&vec, (void *)1);
  darrayPush(&vec, (void *)2);
  darrayPush(&vec, (void *)3);
  darrayPush(&vec, (void *)4);
  darrayPush(&vec, (void *)4);
  darrayPush(&vec, (void *)4);
  darrayPush(&vec, (void *)4);
  darrayPush(&vec, (void *)5);
  darrayPush(&vec, (void *)6);
  darrayPrint("%d", &vec);
  puts("");

  printf("Inserting 10 in 3...\n");
  darrayInsert(&vec, 3, (void *)10);
  darrayPrint("%d", &vec);
  puts("");

  printf("Printf darray at 3 and 4: %d, %d\n", darrayAt(&vec, 3), darrayAt(&vec, 4));
  puts("");

  printf("Popping darray\n");
  printf("Popped: %d\n", darrayPop(&vec));
  darrayPrint("%d", &vec);
  puts("");

  printf("Deleting index 3\n");
  darrayDelete(&vec, 3);
  darrayPrint("%d", &vec);
  puts("");

  printf("Finding 10 in darray: %d\n", darrayFind(&vec, (void *)10));
  printf("Finding 5 in darray: %d\n", darrayFind(&vec, (void *)5));
  darrayPrint("%d", &vec);
  puts("");

  return 0;
}
