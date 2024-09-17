#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHAR 1024

// matrix *matrixNew(unsigned int rows, unsigned int cols) {
//   matrix *mat = malloc(sizeof(matrix));
//   mat->items = malloc(rows * sizeof(double *));
//   for (int i = 0; i < rows; i++) {
//     mat->items[i] = malloc(cols * sizeof(double));
//   }
//   mat->rows = rows;
//   mat->cols = cols;
//   return mat;
// }

matrix *matrixNew(unsigned int rows, unsigned int cols) {
  matrix *mat = malloc(sizeof(matrix));
  if (mat == NULL) {
    return NULL; // Error in matrix allocation
  }

  mat->items = malloc(rows * sizeof(double *));
  if (mat->items == NULL) {
    free(mat);
    return NULL; // Error in row allocation
  }

  for (unsigned int i = 0; i < rows; i++) {
    mat->items[i] = malloc(cols * sizeof(double));
    if (mat->items[i] == NULL) {
      // Free all previously allocated rows
      for (unsigned int j = 0; j < i; j++) {
        free(mat->items[j]);
      }
      free(mat->items);
      free(mat);
      return NULL; // Error in column allocation
    }
  }

  mat->rows = rows;
  mat->cols = cols;
  return mat;
}

matrix *matrixFromDataFrame(dataFrame *df) {
  matrix *mat = matrixNew(df->rows->size, df->header->size);
  for (int i = 0; i < df->rows->size; i++) {
    for (int j = 0; j < df->header->size; j++) {
      mat->items[i][j] = atof(darrayAt(darrayAt(df->rows, i), j));
    }
  }
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

  fprintf(file, "%d %d\n", mat->rows, mat->cols);

  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      fprintf(file, "%f ", mat->items[i][j]);
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

matrix *matrixLoad(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  unsigned int rows = 0;
  unsigned int cols = 0;

  // Read matrix dimensions
  if (fscanf(file, "%u %u", &rows, &cols) != 2) {
    fprintf(stderr, "Error reading matrix dimensions\n");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  // Create a new matrix with the read dimensions
  matrix *mat = matrixNew(rows, cols);
  printf("%d x %d\n", mat->rows, mat->cols);
  if (mat == NULL) {
    perror("Error allocating memory for matrix");
    fclose(file);
    exit(EXIT_FAILURE);
  }

  // Read matrix items
  for (int i = 0; i < rows * cols; i++) {
    if (fscanf(file, "%lf", &mat->items[i / cols][i % cols]) != 1) {
      fprintf(stderr, "Error reading matrix item at row %d, col %d\n", i / cols,
              i % cols);
      fclose(file);
      exit(EXIT_FAILURE);
    }
  }

  fclose(file);
  matrixPrint(mat);
  printf("Loaded %d x %d\n", mat->rows, mat->cols);
  return mat;
}

// matrix *matrixLoad(const char *filename) {
//   FILE *file = fopen(filename, "r");
//   if (file == NULL) {
//     perror("Error opening file");
//     exit(EXIT_FAILURE);
//   }
//
//   char line[MAX_CHAR];
//   unsigned int rows = 0;
//   unsigned int cols = 0;
//   if (fscanf(file, "%d %d", &rows, &cols) != 2) {
//     printf("%d %d\n", rows, cols);
//     perror("Error reading matrix dimensions");
//     fclose(file);
//     exit(EXIT_FAILURE);
//   }
//   matrix *mat = matrixNew(rows, cols);
//
//   for (int i = 0; i < rows * cols; i++) {
//     if (fscanf(file, "%lf", &mat->items[i / cols][i % cols]) != 1) {
//       perror("Error reading matrix item");
//       fclose(file);
//       exit(EXIT_FAILURE);
//     }
//   }
//   fclose(file);
//   return mat;
// }

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
