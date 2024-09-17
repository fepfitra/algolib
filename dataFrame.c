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
  darray *header;
  darray *rows;
} dataFrame;

dataFrame *dataFrameNew() {
  dataFrame *df = malloc(sizeof(dataFrame));
  df->header = darrayNew();
  df->rows = darrayNew();
  return df;
}

darray *parseLine(char *line) {
  bool in_quotes = false;
  char *start = line;
  darray *parsedLine = darrayNew();
  for (char *p = line; *p != '\0'; p++) {
    if (*p == '"') {
      in_quotes = !in_quotes;
    } else if (*p == ',' && !in_quotes) {
      *p = '\0';
      darrayPush(parsedLine, strdup(start));
      start = p + 1;
    }
  }
  darrayPush(parsedLine, strdup(start));
  return parsedLine;
}

dataFrame *dataFrameFromLines(char *ptr, unsigned int size,
                              unsigned int bufferSize) {
  char buffer[bufferSize];
  char *ptrTmp = ptr;

  dataFrame *df = dataFrameNew();
  if (df == NULL) {
    return NULL;
  }

  while (ptrTmp < ptr + size) {
    int chars_read = sscanf(ptrTmp, "%[^\n]", buffer);
    ptrTmp += strlen(buffer) + 1;
    darray *parsed = parseLine(buffer);

#ifdef DEBUG
    fprintf(stderr, "parsing line %d, found length %d\n", counter,
            parsed->size);
#endif

    darrayPush(df->rows, parsed);
  }

#ifdef DEBUG
  fprintf(stderr, "Loaded %d x %d\n", df->rows->size, df->header->size);
#endif
  return df;
}

dataFrame *dataFrameFromCSV(const char *fileName, unsigned int bufferSize) {
  char *f;
  unsigned int size;
  struct stat s;

  int fd = open(fileName, O_RDONLY);
  if (fd == -1) {
    perror("Error opening file");
    return NULL;
  }

  if (fstat(fd, &s) == -1) {
    perror("Error getting file size");
    close(fd);
    return NULL;
  }
  size = s.st_size;

  f = (char *)mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (f == MAP_FAILED) {
    perror("Error mapping file");
    close(fd);
    return NULL;
  }

  dataFrame *df = dataFrameFromLines(f, size, bufferSize);

  munmap(f, size);
  close(fd);
  return df;
}

