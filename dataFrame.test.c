#include "dataFrame.h"
#include <stdio.h>

int main() {
  // dataFrame *df = dataFrameFromCSV(
  //     "./../../Projects/InformationRetrieval/SimpleIR/News.csv", 0xfffff);
  dataFrame *df = dataFrameFromCSV("./test.csv", 0xfffff);
  // darrayPrint("%s", df->header);
  // printf("Rows: %d\n", df->rows->size);
  // darrayPrint("%s", darrayAt(df->rows, 1));
  for (int i=0; i<df->rows->size; i++) {
    darrayPrint("%.10s", darrayAt(df->rows, i));
  }
  return 0;
}
