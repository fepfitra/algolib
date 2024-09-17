#include "dataFrame.h"

int main() {
  dataFrame *df = dataFrameFromCSV(
      "./../../Projects/InformationRetrieval/SimpleIR/News.csv", 0xfffff);
  darrayPrint("%s", df->columns);
  return 0;
}
