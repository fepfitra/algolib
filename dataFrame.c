#include "dataFrame.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

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

void dataFramePrepareHeader(dataFrame *df) {
  df->header = darrayAt(df->rows, 0);
  darrayDelete(df->rows, 0);
}

void dataFrameDestroy(dataFrame *df) {
  for (int i = 0; i < df->rows->size; i++) {
    darray *row = darrayAt(df->rows, i);
    for (int j = 0; j < row->size; j++) {
      free(darrayAt(row, j));
    }
    darrayDestroy(row);
  }
  darrayDestroy(df->rows);
  darrayDestroy(df->header);
  free(df);
}

void dataFramePrint(dataFrame *df, unsigned int n) {
  if (n > df->rows->size) {
    n = df->rows->size;
  }

  for (int i = 0; i < n; i++) {
    darrayPrint("%s", darrayAt(df->rows, i));
  }
}

void dataFramePrintHeader(dataFrame *df) { darrayPrint("%s", df->header); }

void dataFramePrintRow(dataFrame *df, unsigned int offset, unsigned int range) {
  if (offset + range > df->rows->size) {
    range = df->rows->size - offset;
  }

  for (int i = offset; i < offset + range; i++) {
    darrayPrint("%s", darrayAt(df->rows, i));
  }
}
