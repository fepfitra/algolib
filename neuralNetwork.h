#pragma once
#include "matrix.h"
#include "nnLayer.h"
#include <math.h>

typedef struct neuralNetwork {
  nnLayer *layer;
  double learningRate;
  double delta;
} neuralNetwork;

enum errorFunction {
  MAE,
  MSE,
};

double per2(double x);
double activationSigmoid(double x);

neuralNetwork *neuralNetworkNew(nnLayer *layer, double learningRate);
void neuralNetworkPrint(neuralNetwork *nn);
neuralNetwork *neuralNetworkTrain(neuralNetwork *nn, matrix *x, matrix *y);
double neuralNetworkForwardPass(neuralNetwork *nn, matrix *x, matrix *y);
neuralNetwork *neuralNetworkBackwardPass(neuralNetwork *nn, matrix *x,
                                         matrix *y);
double neuralNetworkForwardPass(neuralNetwork *nn, matrix *x, matrix *y);
double neuralNetworkCentralDiff(neuralNetwork *nn, matrix *x, matrix *y,
                                double *target);
double neuralNetworkNormD(neuralNetwork *nn);
