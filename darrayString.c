#include "darray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void darrayStringSplit(darray *vec, const char *str, const char *delim) {
  unsigned int length = strlen(str);
  unsigned int offset = 0;
  for (unsigned int i = 0; i < length; i++) {
    if (i == length - 1) {
      char *item = strndup(str+offset, i-offset+1);
      printf("item: %s\n", item);
      darrayPush(vec, item);
    }
    if (str[i] == *delim) {
      char *item = strndup(str+offset, i-offset);
      printf("item: %s\n", item);
      darrayPush(vec, item);
      offset = i+1;
    }
  }
}
