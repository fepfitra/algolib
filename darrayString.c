#include "darray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void darrayStringSplit(darray *vec, char *str, const char *delim) {
  unsigned int length = strlen(str);
  unsigned int offset = 0;
  printf("parsing: %s\n", str);
  for (unsigned int i = 0; i < length; i++) {
    if (i == length - 1) {
      darrayPush(vec, str + offset);
    } else if (str[i] == delim[0]) {
      str[i] = '\0';
      darrayPush(vec, str + offset);
      offset = i + 1;
    }
  }
}
