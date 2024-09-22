#include "neuralNetwork.h"
#include "float.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

double per2(double x) { return x / 2; }
double activationSigmoid(double x) { return 1 / (1 + exp(-x)); }

neuralNetwork *neuralNetworkNew(nnLayer *layer, double learningRate) {
  neuralNetwork *nn = malloc(sizeof(neuralNetwork));
  nn->layer = layer;
  nn->learningRate = learningRate;
  nn->delta = 1e-6;
  return nn;
}

void neuralNetworkPrint(neuralNetwork *nn) {
  nnLayerPrint(nn->layer);
  printf("Learning rate:\t%0.10lf\n", nn->learningRate);
}

neuralNetwork *neuralNetworkTrain(neuralNetwork *nn, matrix *x, matrix *y) {
  for (int i = 0; i < 1; i++) {
    matrix *features = matrixSlice(x, i, i + 1, 0, x->cols);
    matrix *target = matrixSlice(y, i, i + 1, 0, y->cols);
    // double error = neuralNetworkForwardPass(nn, features, target);
    // printf("Error: %f\n", error);
    for (int j = 0; j < 4; j++) {
      nn = neuralNetworkBackwardPass(nn, features, target);
      double normD = neuralNetworkNormD(nn);
      // printf("%lf\n", normD);
      matrixPrint(nn->layer->weights);
    }
  }

  return nn;
}

neuralNetwork *neuralNetworkBackwardPass(neuralNetwork *nn, matrix *x,
                                         matrix *y) {
  nnLayer *layer = nn->layer;
  while (layer != NULL) {
    for (int i = 0; i < layer->weights->rows; i++) {
      for (int j = 0; j < layer->weights->cols; j++) {
        double *target = &layer->weights->items[i][j];
        double diff = neuralNetworkCentralDiff(nn, x, y, target);
        layer->dWeights->items[i][j] = diff;
      }
    }
    layer = layer->next;
  }
  layer = nn->layer;

  while (layer != NULL) {
    for (int i = 0; i < layer->weights->rows; i++) {
      for (int j = 0; j < layer->weights->cols; j++) {
        layer->weights->items[i][j] -=
            nn->learningRate * layer->dWeights->items[i][j];
      }
    }
    layer = layer->next;
  }
  return nn;
}

double neuralNetworkNormD(neuralNetwork *nn) {

  double normD = 0;
  nnLayer *layer = nn->layer;

  while (layer != NULL) {
    for (int i = 0; i < layer->weights->rows; i++) {
      for (int j = 0; j < layer->weights->cols; j++) {
        normD += pow(layer->dWeights->items[i][j], 2);
      }
    }
    layer = layer->next;
  }
  return normD;
};

double neuralNetworkCentralDiff(neuralNetwork *nn, matrix *x, matrix *y,
                                double *target) {
  double tmp = *target;
  double delta = 1e-7;

  *target = tmp - delta;
  double error1 = neuralNetworkForwardPass(nn, x, y);

  *target = tmp + delta;
  double error2 = neuralNetworkForwardPass(nn, x, y);

  *target = tmp;
  return (error2 - error1) / (2 * delta);
}

double neuralNetworkForwardPass(neuralNetwork *nn, matrix *x, matrix *y) {
  nnLayer *layer = nn->layer;

  matrix *tmp = matrixFromMatrix(x);

  while (layer != NULL) {
    matrix *weights = matrixFromMatrix(layer->weights);
    matrix *biases = matrixFromMatrix(layer->biases);

    tmp = matrixDot(tmp, weights, 1);
    tmp = matrixAdd(tmp, biases, 1);
    layer = layer->next;
  }

  double cost = 0;
  for (int i = 0; i < tmp->cols; i++) {
    cost += pow(y->items[0][i] - tmp->items[0][i], 2);
  }
  return cost / 2;
  // return nn;
}
