#include "darrayString.h"
#include <stdlib.h>
#include <string.h>

int main() {
  darray vec;
  darrayInit(&vec);

  darrayStringSplit(&vec, "Hello\nWor\nld", "\n");
  darrayPrint("%s", &vec);
}
