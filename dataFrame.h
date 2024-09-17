#include "darray.h"

typedef struct {
  darray *columns;
  darray *rows;
} dataFrame;

dataFrame *dataFrameNew();
dataFrame *dataFrameFromCSV(const char *fileName, unsigned int bufferSize);
