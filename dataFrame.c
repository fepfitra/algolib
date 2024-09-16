#include "darray.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  darray *columns;
  darray *rows;
} dataFrame;

void dataFrameInit(dataFrame *df) {
  df->columns = malloc(sizeof(darray));
  df->rows = malloc(sizeof(darray));
  darrayInit(df->columns, 1);
  darrayInit(df->rows, 1);
}

darray *parseLine(char *line) {
  bool in_quotes = false;
  char *start = line;
  darray *vec = malloc(sizeof(darray));
  darrayInit(vec, 1);
  for (char *p = line; *p != '\0'; p++) {
    if (*p == '"') {
      in_quotes = !in_quotes;
    } else if (*p == ',' && !in_quotes) {
      *p = '\0';
      darrayPush(vec, strdup(start));
      start = p + 1;
    }
  }
  darrayPush(vec, strdup(start));
  return vec;
}

void readLines(dataFrame *df, char *fileName, unsigned int fileSize,
               unsigned int bufferSize) {
  char buffer[bufferSize];
  char *ptr = fileName;
  char delim;
  unsigned int counter = 0;

  darray vec;
  darrayInit(&vec, 1);
  while (ptr < fileName + fileSize) {
    int chars_read = sscanf(ptr, "%[^\n]", buffer);
    ptr += strlen(buffer) + 1;

    counter++;
    darray *parsed = parseLine(buffer);
    fprintf(stderr,"parsing line %d, found length %d\n", counter, parsed->size);

    darrayPush(df->rows, parsed);
  }
  df->rows->storeHeap = 0;
  df->columns = (darray *)darrayAt(df->rows, 0);
  darrayDelete(df->rows, 0);
  df->rows->storeHeap = 1;
  fprintf(stderr,"Loaded %d x %d\n", df->rows->size, df->columns->size);
}

void dataFrameLoadFromCSV(dataFrame *df, const char *fileName,
                          unsigned int bufferSize) {
  char *f;
  unsigned int size;
  struct stat s;

  int fd = open(fileName, O_RDONLY);
  if (fd == -1) {
    perror("Error opening file");
    return;
  }

  if (fstat(fd, &s) == -1) {
    perror("Error getting file size");
    close(fd);
    return;
  }
  size = s.st_size;

  f = (char *)mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (f == MAP_FAILED) {
    perror("Error mapping file");
    close(fd);
    return;
  }

  readLines(df, f, size, bufferSize);

  munmap(f, size);
  close(fd);
}
