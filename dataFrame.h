#pragma once
#include "darray.h"

typedef struct {
  darray *header;
  darray *rows;
} dataFrame;

dataFrame *dataFrameNew();
dataFrame *dataFrameFromCSV(const char *fileName, unsigned int bufferSize);
void dataFramePrepareHeader(dataFrame *df);
void dataFrameDestroy(dataFrame *df);
void dataFramePrint(dataFrame *df, unsigned int n);
void dataFramePrintHeader(dataFrame *df);
void dataFramePrintRow(dataFrame *df, unsigned int offset, unsigned int range);
void dataFrameReplaceByString(dataFrame *df, const char *column, const char *oldValue, const char *newValue);
void dataFrameReplaceByIndex(dataFrame *df, unsigned int column, const char *oldValue, const char *newValue);
