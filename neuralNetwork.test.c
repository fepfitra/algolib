#include "neuralNetwork.h"
#include <stdio.h>
#include "nnLayer.h"

double test(double x) { return 2 * x; }

int main() {
  matrix *mat = matrixLoad("./matrix.save");
  mat = matrixShuffle(mat, 42, 0);

  matrix *y = matrixSlice(mat, 0, mat->rows, 0, 1);
  matrix *x = matrixSlice(mat, 0, mat->rows, 1, mat->cols);

  int split = (int)(mat->rows * 0.8);
  matrix *x_train = matrixSlice(x, 0, split, 0, x->cols);
  matrix *y_train = matrixSlice(y, 0, split, 0, y->cols);
  matrix *x_test = matrixSlice(x, split, x->rows, 0, x->cols);
  matrix *y_test = matrixSlice(y, split, y->rows, 0, y->cols);

  x = matrixDestroy(x);
  y = matrixDestroy(y);
  mat = matrixDestroy(mat);

  nnLayer *layers = nnLayerNew(x_train->cols, 64);
  layers->next = nnLayerNew(64, 32);
  layers->next->next = nnLayerNew(32, 32);
  layers->next->next->next = nnLayerNew(32, y_train->cols);

  neuralNetwork *nn = neuralNetworkNew(layers, 1e-8);
  nn = neuralNetworkTrain(nn, x_train, y_train);
  return 0;

  // self->alpha = 1e-7;
  // self->delta = 1e-6;
}
