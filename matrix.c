#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 1024

matrix *matrixNew(unsigned int rows, unsigned int cols) {
  matrix *mat = malloc(sizeof(matrix));
  mat->items = malloc(rows * sizeof(double *));
  for (int i = 0; i < rows; i++) {
    mat->items[i] = malloc(cols * sizeof(double));
  }
  mat->rows = rows;
  mat->cols = cols;
  return mat;
}

void matrixFill(matrix *mat, double value) {
  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      mat->items[i][j] = value;
    }
  }
}

void matrixDestroy(matrix *mat) {
  for (int i = 0; i < mat->rows; i++) {
    free(mat->items[i]);
  }
  free(mat->items);
  free(mat);
  mat = NULL;
}

void matrixPrint(matrix *mat) {
  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      printf("%f ", mat->items[i][j]);
    }
    printf("\n");
  }
}

void matrixCopy(matrix *src, matrix *dst) {
  if (src->rows != dst->rows || src->cols != dst->cols) {
    fprintf(stderr, "Matrix dimensions do not match\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < src->rows; i++) {
    for (int j = 0; j < src->cols; j++) {
      dst->items[i][j] = src->items[i][j];
    }
  }
}

void matrixSave(matrix *mat, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      fprintf(file, "%f ", mat->items[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

double uniformDistribution(double low, double high) {
  double diff = high - low;
  int scale = 10000;
  int scaledDiff = (int)(diff * scale);
  return low + (1.0 * (rand() % scaledDiff)) / scale;
}

void matrixRandomize(matrix *mat, int n) {
  double low = -1.0 / sqrt(n);
  double high = 1.0 / sqrt(n);
  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      mat->items[i][j] = uniformDistribution(low, high);
    }
  }
}

int matrixArgmax(matrix *mat) {
  int argmax = 0;
  double max = mat->items[0][0];
  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      if (mat->items[i][j] > max) {
        max = mat->items[i][j];
        argmax = i * mat->cols + j;
      }
    }
  }
  return argmax;
}

matrix *matrixFlatten(matrix *mat, int axis) {
  matrix *mat2;
  if (axis == 0) {
    mat2 = matrixNew(mat->rows * mat->cols, 1);
  } else if (axis == 1) {
    mat2 = matrixNew(1, mat->rows * mat->cols);
  } else {
    fprintf(stderr, "Invalid axis\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      if (axis == 0) {
        mat2->items[i * mat->cols + j][0] = mat->items[i][j];
      } else {
        mat2->items[0][i * mat->cols + j] = mat->items[i][j];
      }
    }
  }
  return mat2;
}

int matrixCheckDimenstions(matrix *a, matrix *b) {
  return a->rows == b->rows && a->cols == b->cols;
}

matrix *matrixMultiply(matrix *a, matrix *b) {
  if (matrixCheckDimenstions(a, b)) {
    matrix *c = matrixNew(a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
      for (int j = 0; j < a->cols; j++) {
        c->items[i][j] = a->items[i][j] * b->items[i][j];
      }
    }
    return c;
  } else {
    fprintf(stderr, "Matrix dimensions do not match\n");
    exit(EXIT_FAILURE);
  }
}

matrix *matrixAdd(matrix *a, matrix *b) {
  if (matrixCheckDimenstions(a, b)) {
    matrix *c = matrixNew(a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
      for (int j = 0; j < a->cols; j++) {
        c->items[i][j] = a->items[i][j] + b->items[i][j];
      }
    }
    return c;
  } else {
    fprintf(stderr, "Matrix dimensions do not match\n");
    exit(EXIT_FAILURE);
  }
}

matrix *matrixSubtract(matrix *a, matrix *b) {
  if (matrixCheckDimenstions(a, b)) {
    matrix *c = matrixNew(a->rows, a->cols);
    for (int i = 0; i < a->rows; i++) {
      for (int j = 0; j < a->cols; j++) {
        c->items[i][j] = a->items[i][j] - b->items[i][j];
      }
    }
    return c;
  } else {
    fprintf(stderr, "Matrix dimensions do not match\n");
    exit(EXIT_FAILURE);
  }
}

matrix *matrixApply(matrix *mat, double (*fn)(double)) {
  matrix *mat2 = matrixNew(mat->rows, mat->cols);
  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      mat2->items[i][j] = (*fn)(mat->items[i][j]);
    }
  }
  return mat2;
}

matrix *matrixDot(matrix *a, matrix *b) {
  if (a->cols != b->rows) {
    fprintf(stderr, "Matrix dimensions do not match\n");
    exit(EXIT_FAILURE);
  }

  matrix *c = matrixNew(a->rows, b->cols);
  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < b->cols; j++) {
      c->items[i][j] = 0;
      for (int k = 0; k < a->cols; k++) {
        c->items[i][j] += a->items[i][k] * b->items[k][j];
      }
    }
  }
  return c;
}

matrix *matrixScale(matrix *a, double n) {
  matrix *c = matrixNew(a->rows, a->cols);
  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < a->cols; j++) {
      c->items[i][j] = a->items[i][j] * n;
    }
  }
  return c;
}

matrix *MatrixAddScalar(matrix *a, double n) {
  matrix *c = matrixNew(a->rows, a->cols);
  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < a->cols; j++) {
      c->items[i][j] = a->items[i][j] + n;
    }
  }
  return c;
}

matrix *matrixTranspose(matrix *a) {
  matrix *c = matrixNew(a->cols, a->rows);
  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < a->cols; j++) {
      c->items[j][i] = a->items[i][j];
    }
  }
  return c;
}
