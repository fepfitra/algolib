#include "darray.h"

typedef struct {
  unsigned int size;
  darray *columns;
  darray *items;
} dataFrame ;

void dataFrameInit(dataFrame *df);
void dataFrameLoadFromCSV(dataFrame *df, const char *fileName);
