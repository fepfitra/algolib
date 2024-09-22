#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "queryParser.h"

int main() {
  char c[] = "a or b or c and d or (e and f) or ( g and not h )";

  darray *tokens = queryTokenize(c);

  int index = 0;
  queryTree *root = queryBuildTree(tokens, &index, 0);

  printf("Query Tree Structure:\n");
  queryTreePrint(root);
  printf("\n");

  return 0;
}

