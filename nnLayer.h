#pragma once
#include "matrix.h"

struct nnLayer;

typedef struct nnLayer {
  matrix *weights;
  matrix *biases;
  matrix *dWeights;
  matrix *dBiases;
  struct nnLayer *next;
} nnLayer;

nnLayer *nnLayerNew(size_t input, size_t output);
void nnLayerPrint(nnLayer *layer);

