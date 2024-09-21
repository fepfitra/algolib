#include "dataFrame.h"
#include <stdio.h>
#pragma once

typedef struct {
  double **items;
  size_t rows;
  size_t cols;
} matrix;

matrix *matrixNew(size_t rows, size_t cols);
matrix *matrixFromDataFrame(dataFrame *df);
matrix *matrixFromMatrix(matrix *mat);
matrix *matrixFill(matrix *mat, double value);
matrix *matrixDestroy(matrix *mat);
void matrixPrint(matrix *mat);
void matrixCopy(matrix *src, matrix *dst);
void matrixSave(matrix *mat, const char *filename);
matrix *matrixShuffle(matrix *mat, int state, size_t destroyArgs);
matrix *matrixLoad(const char *filename);
matrix *matrixRandomize(matrix *mat, int n);

int matrixArgmax(matrix *mat);

matrix *matrixFlatten(matrix *mat, size_t axis, size_t destroyArgs);
matrix *matrixSlice(matrix *mat, size_t rowStart, size_t rowEnd, size_t colStart,
                    size_t colEnd);

matrix *matrixMultiply(matrix *a, matrix *b, size_t destroyArgs);
matrix *matrixAdd(matrix *a, matrix *b, size_t destroyArgs);
matrix *matrixSubtract(matrix *a, matrix *b, size_t destroyArgs);
matrix *matrixApply(matrix *mat, double (*fn)(double), size_t destroyArgs);
matrix *matrixDot(matrix *a, matrix *b, size_t destroyArgs);
matrix *matrixScale(matrix *a, double n, size_t destroyArgs);
matrix *matrixAddScalar(matrix *a, double n, size_t destroyArgs);
matrix *matrixTranspose(matrix *a, size_t destroyArgs);
