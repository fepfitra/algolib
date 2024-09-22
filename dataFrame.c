#include "dataFrame.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

char *trimWhitespace(char *str) {
  char *end;

  // Trim leading spaces
  while (isspace((unsigned char)*str))
    str++;

  if (*str == 0) // All spaces?
    return str;

  // Trim trailing spaces
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  // Write new null terminator
  *(end + 1) = 0;

  return str;
}

dataFrame *dataFrameNew() {
  dataFrame *df = malloc(sizeof(dataFrame));
  df->header = darrayNew();
  df->rows = darrayNew();
  return df;
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
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
      darrayPush(parsedLine, strdup(trimwhitespace(start)));
      start = p + 1;
    }
  }
  darrayPush(parsedLine, strdup(trimwhitespace(start)));
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

void dataFramePrintColumn(dataFrame *df, const char *column,
                          unsigned int offset, unsigned int range) {
  int columnIndex = -1;
  for (int i = 0; i < df->header->size; i++) {
    // printf("Comparing %s with %s\n", darrayAt(df->header, i), column);
    if (strcmp(darrayAt(df->header, i), column) == 0) {
      columnIndex = i;
      break;
    }
  }
  if (columnIndex == -1) {
    fprintf(stderr, "Column %s not found\n", column);
    return;
  }

  if (offset + range > df->rows->size) {
    range = df->rows->size - offset;
  }

  printf("[");
  for (int i = offset; i < offset + range; i++) {
    darray *row = darrayAt(df->rows, i);
    printf("%s", darrayAt(row, columnIndex));
    if (i < offset + range - 1) {
      printf(", ");
    }
  }
  printf("]");
}

void dataFrameReplaceByIndex(dataFrame *df, unsigned int column,
                             const char *oldValue, const char *newValue) {
  for (int i = 0; i < df->rows->size; i++) {
    darray *row = darrayAt(df->rows, i);
    if (strcmp(darrayAt(row, column), oldValue) == 0) {
      free(darrayAt(row, column));
      darraySet(row, column, strdup(newValue));
    }
  }
}

void dataFrameReplaceByString(dataFrame *df, const char *column,
                              const char *oldValue, const char *newValue) {
  int columnIndex = -1;
  for (int i = 0; i < df->header->size; i++) {
    if (strcmp(darrayAt(df->header, i), column) == 0) {
      columnIndex = i;
      break;
    }
  }
  if (columnIndex == -1) {
    fprintf(stderr, "Column %s not found\n", column);
    return;
  }

  dataFrameReplaceByIndex(df, columnIndex, oldValue, newValue);
}

void dataFramePrintUniqueByIndex(dataFrame *df, unsigned int column) {
  darray *unique = darrayNew();
  for (int i = 0; i < df->rows->size; i++) {
    unsigned int isExists = 0;
    darray *row = darrayAt(df->rows, i);
    char *value = darrayAt(row, column);
    for (int j = 0; j < unique->size; j++) {
      if (strcmp(darrayAt(unique, j), value) == 0) {
        isExists = 1;
        break;
        // printf("Value %s exists\n", value);
      }
    }
    if (!isExists) {
      darrayPush(unique, strdup(value));
    }
  }
  printf("%s: ", darrayAt(df->header, column));
  darrayPrint("%s", unique);
  for (int j = 0; j < unique->size; j++) {
    free(darrayAt(unique, j));
  }
  darrayDestroy(unique);
}

void dataFramePrintUniqueByString(dataFrame *df, const char *column) {
  int columnIndex = -1;
  for (int i = 0; i < df->header->size; i++) {
    unsigned int res = strcmp(darrayAt(df->header, i), column);
    // printf("Comparing %s: %d\n", darrayAt(df->header, i), res);
    if (res == 0) {
      columnIndex = i;
      break;
    }
    // if (strcmp(darrayAt(df->header, i), column) == 0) {
    //   columnIndex = i;
    //   break;
    // }
  }
  if (columnIndex == -1) {
    fprintf(stderr, "Column %s not found\n", column);
    return;
  }

  dataFramePrintUniqueByIndex(df, columnIndex);
}
