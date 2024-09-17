#pragma once

typedef struct {
  double **items;
  unsigned int rows;
  unsigned int cols;
} matrix;

matrix *matrixNew(unsigned int rows, unsigned int cols);
void matrixFill(matrix *mat, double value);
void matrixDestroy(matrix *mat);
void matrixPrint(matrix *mat);
void matrixCopy(matrix *src, matrix *dst);
void matrixSave(matrix *mat, const char *filename);
void matrixRandomize(matrix *mat, int n);
int matrixArgmax(matrix *mat);
matrix *matrixFlatten(matrix *mat, int axis);

matrix *matrixMultiply(matrix *a, matrix *b);
matrix *matrixAdd(matrix *a, matrix *b);
matrix *matrixSubtract(matrix *a, matrix *b);
matrix *matrixApply(matrix *mat, double (*fn)(double));
matrix *matrixDot(matrix *a, matrix *b);
matrix *matrixScale(matrix *a, double n);
matrix *matrixAddScalar(matrix *a, double n);
matrix *matrixTranspose(matrix *a);
