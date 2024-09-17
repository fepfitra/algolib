#include "matrix.h"
#include <stdio.h>

double test(double a) {
  return a * 2;
}

int main() {
  matrix *mat = matrixNew(3, 3);
  matrixFill(mat, 1.0);
  matrixPrint(mat);
  matrixRandomize(mat, 10);
  matrixPrint(mat);
  printf("Argmax: %d\n", matrixArgmax(mat));
  // matrixSave(mat, "matrix.save");
  // mat = matrixFlatten(mat, 1);
  // matrixPrint(mat);

  matrix *mat2 = matrixNew(3, 3);
  matrixFill(mat2, 2.0);
  matrixPrint(mat2);

  matrix *mat3 = matrixMultiply(mat, mat2);
  matrixPrint(mat3);

  //apply
  matrix *mat4 = matrixApply(mat3, test);
  matrixPrint(mat4);

  mat4 = matrixFlatten(mat4, 1);
  matrixPrint(mat4);

      return 0;
}
