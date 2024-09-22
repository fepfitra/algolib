#include "matrix.h"
#include "dataFrame.h"
#include <stdio.h>

#define name(x) #x

double test(double a) { return a * 2; }

int main() {

  dataFrame *df = dataFrameFromCSV("./Housing.csv", 0xff);
  dataFramePrepareHeader(df);
  printf("Printing header...\n");
  dataFramePrintHeader(df);

  printf("\nPrinting 5 rows...\n");
  dataFramePrint(df, 5);

  printf("\nChecking value types...");
  dataFramePrintUniqueByString(df, "bedrooms");
  dataFramePrintUniqueByString(df, "bathrooms");
  dataFramePrintUniqueByString(df, "stories");
  dataFramePrintUniqueByString(df, "mainroad");
  dataFramePrintUniqueByString(df, "guestroom");
  dataFramePrintUniqueByString(df, "basement");
  dataFramePrintUniqueByString(df, "hotwaterheating");
  dataFramePrintUniqueByString(df, "airconditioning");
  dataFramePrintUniqueByString(df, "parking");
  dataFramePrintUniqueByString(df, "prefarea");
  dataFramePrintUniqueByString(df, "furnishingstatus");

  printf("\nReplacing values...\n");
  dataFrameReplaceByString(df, "mainroad", "yes", "1");
  dataFrameReplaceByString(df, "mainroad", "no", "0");
  dataFrameReplaceByString(df, "guestroom", "yes", "1");
  dataFrameReplaceByString(df, "guestroom", "no", "0");
  dataFrameReplaceByString(df, "basement", "yes", "1");
  dataFrameReplaceByString(df, "basement", "no", "0");
  dataFrameReplaceByString(df, "hotwaterheating", "yes", "1");
  dataFrameReplaceByString(df, "hotwaterheating", "no", "0");
  dataFrameReplaceByString(df, "airconditioning", "yes", "1");
  dataFrameReplaceByString(df, "airconditioning", "no", "0");
  dataFrameReplaceByString(df, "prefarea", "yes", "1");
  dataFrameReplaceByString(df, "prefarea", "no", "0");
  dataFrameReplaceByString(df, "furnishingstatus", "furnished", "1");
  dataFrameReplaceByString(df, "furnishingstatus", "semi-furnished", "2");
  dataFrameReplaceByString(df, "furnishingstatus", "unfurnished", "3");

  printf("\nChecking value types...\n");
  dataFramePrintUniqueByString(df, "bedrooms");
  dataFramePrintUniqueByString(df, "bathrooms");
  dataFramePrintUniqueByString(df, "stories");
  dataFramePrintUniqueByString(df, "mainroad");
  dataFramePrintUniqueByString(df, "guestroom");
  dataFramePrintUniqueByString(df, "basement");
  dataFramePrintUniqueByString(df, "hotwaterheating");
  dataFramePrintUniqueByString(df, "airconditioning");
  dataFramePrintUniqueByString(df, "parking");
  dataFramePrintUniqueByString(df, "prefarea");
  dataFramePrintUniqueByString(df, "furnishingstatus");

  printf("\nPrinting 5 rows...\n");
  dataFramePrint(df, 5);
  printf("%d x %d\n", df->rows->size, df->header->size);

  matrix *mat = matrixFromDataFrame(df);
  dataFrameDestroy(df);
  df = NULL;

  matrixPrint(mat);
  printf("%lu x %lu\n", mat->rows, mat->cols);
  matrixSave(mat, "./matrix.save");
  mat = matrixDestroy(mat);
  printf("%p\n", mat);
  //
  matrix *mat2 = matrixLoad("./matrix.save");
  matrixPrint(mat2);
  printf("%lu x %lu\n", mat2->rows, mat2->cols);
  //
  matrix *mat3 = matrixFromMatrix(mat2);
  mat3 = matrixApply(mat3, test, 0);

  mat2 = matrixFlatten(mat2, 0, 1);
  matrixPrint(mat2);
  return 0;
}
