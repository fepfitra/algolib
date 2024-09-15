#include "darrayString.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
  unsigned int row;
  unsigned int column;
  darray *columns;
  darray *items;
} dataFrame;

void dataFrameInit(dataFrame *df) {
  df->row = 0;
  df->column = 0;
  df->columns = malloc(sizeof(darray));
  df->items = malloc(sizeof(darray));
  darrayInit(df->columns);
  darrayInit(df->items);
}


void dataFrameLoadFromCSV(dataFrame *df, const char *fileName) {
  char *f;
  unsigned int size;
  struct stat s;
  
  // Open the file
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

  darray lines;
  darrayInit(&lines);
  darrayStringSplit(&lines, f, "\n");
  // darrayPrint("%s", &lines);

  close(fd);
}
