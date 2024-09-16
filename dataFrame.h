#include "darray.h"

typedef struct {
  darray *columns;
  darray *rows;
} dataFrame ;

void dataFrameInit(dataFrame *df);
void dataFrameLoadFromCSV(dataFrame *df, const char *fileName, unsigned int bufferSize);
