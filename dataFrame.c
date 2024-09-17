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

void readLines(dataFrame *df, char *fileName, unsigned int fileSize,
               unsigned int bufferSize) {
  char buffer[bufferSize];
  char *ptr = fileName;
  char delim;
  unsigned int counter = 0;

  while (ptr < fileName + fileSize) {
    int chars_read = sscanf(ptr, "%[^\n]", buffer);
    ptr += strlen(buffer) + 1;

    counter++;
    darray *parsed = parseLine(buffer);

#ifdef DEBUG
    fprintf(stderr, "parsing line %d, found length %d\n", counter,
            parsed->size);
#endif

    darrayPush(df->rows, parsed);
  }
  df->header = (darray *)darrayAt(df->rows, 0);
  darrayDelete(df->rows, 0);

#ifdef DEBUG
  fprintf(stderr, "Loaded %d x %d\n", df->rows->size, df->columns->size);
#endif
}

dataFrame *dataFrameFromCSV(const char *fileName, unsigned int bufferSize) {
  char *f;
  unsigned int size;
  struct stat s;

  dataFrame *df = dataFrameNew();

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

  readLines(df, f, size, bufferSize);

  munmap(f, size);
  close(fd);
  return df;
}
