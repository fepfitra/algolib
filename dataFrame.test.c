#include<stdio.h>
#include "dataFrame.h"

int main() {
  dataFrame df;
  dataFrameInit(&df);
  dataFrameLoadFromCSV(&df, "./dataFrame.example.csv");
  return 0;
}
