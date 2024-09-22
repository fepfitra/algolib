#pragma once
#include "darray.h"

typedef struct queryTree {
  char *value;
  struct queryTree *left;
  struct queryTree *right;
} queryTree;

darray *queryTokenize(char *query);

queryTree *queryNewNode(char *value);

queryTree *queryBuildTree(darray *tokens, int *index, int precedence);
void queryTreePrint(queryTree *root);
