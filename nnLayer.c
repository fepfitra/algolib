#include "nnLayer.h"
#include <stdlib.h>

nnLayer *nnLayerNew(size_t input, size_t output) {
  nnLayer *layer = malloc(sizeof(nnLayer));
  layer->weights = matrixNew(input, output);
  layer->weights = matrixFill(layer->weights, 0.1);

  layer->biases = matrixNew(1, output);
  layer->biases = matrixFill(layer->biases, 0.1);

  layer->dWeights = matrixNew(input, output);
  layer->dBiases = matrixNew(1, output);

  layer->next = NULL;
  return layer;
}

void nnLayerPrint(nnLayer *layer) {
  while (layer != NULL) {
    matrixPrintDim(layer->weights);
    layer = layer->next;
  }
}
