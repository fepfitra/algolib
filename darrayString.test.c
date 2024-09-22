#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "darray.h"

unsigned int parseCSVLine(char *line) {
  bool in_quotes = false;
  char *start = line;
  unsigned int itemCount = 0;
  for (char *p = line; *p != '\0'; p++) {
    if (*p == '"') {
      in_quotes = !in_quotes;
    } else if (*p == ',' && !in_quotes) {
      *p = '\0';
      itemCount++;
      printf("%s, ", start);
      start = p + 1;
    }
  }
  itemCount++;
  printf("%s", start);
  // printf("\nItem count: %d\n", itemCount);
  return itemCount;
}

int main() {

  darray vec;
  darrayInit(&vec);

  char str[4096];
  char delim;
  // unsigned int itemCount = 0;

  while (scanf("%[^\n]%c", str, &delim) != EOF) {
    printf("Parsing line: %s\n", str);
    // parseCSVLine(str);
    darrayPush(&vec, (void *)parseCSVLine(str));
    // itemCount++;
  }
  darrayPrint("%d", &vec);

  // printf("\nItem count: %d\n", itemCount);

  return 0;
}
