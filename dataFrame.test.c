#include "dataFrame.h"

int main() {
  dataFrame df;
  dataFrameInit(&df);
  // dataFrameLoadFromCSV(&df,"./test.csv", 0xfffff);
  dataFrameLoadFromCSV(&df, "../../../share/IR/SimpleIR/News.csv", 0xfffff);
  darrayPrint("%s", df.columns);
  return 0;
}
