#include"queryParser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

darray *queryTokenize(char *query) {
  char buffer[100];
  char *ptr = query;
  darray *tokens = darrayNew();

  while (*ptr != '\0') {
    while (isspace(*ptr)) {
      ptr++;
    }

    if (*ptr == '(') {
      darrayPush(tokens, strndup("(", 1));
      ptr++;
      continue;
    }
    if (*ptr == ')') {
      darrayPush(tokens, strndup(")", 1));
      ptr++;
      continue;
    }
    sscanf(ptr, "%s", buffer);
    size_t len = strlen(buffer);

    if (buffer[len - 1] == ')') {
      buffer[len - 1] = '\0';
      darrayPush(tokens, strndup(buffer, len - 1));
      darrayPush(tokens, strndup(")", 1));
    } else {
      darrayPush(tokens, strndup(buffer, len));
    }

    ptr += len;

    while (isspace(*ptr)) {
      ptr++;
    }
  }
  return tokens;
}

queryTree *newNode(char *value) {
  queryTree *node = (queryTree *)malloc(sizeof(queryTree));
  node->value = strdup(value);
  node->left = NULL;
  node->right = NULL;
  return node;
}

queryTree *queryBuildTree(darray *tokens, int *index, int precedence);

queryTree *parseTerm(darray *tokens, int *index) {
  queryTree *root = NULL;

  char *token = darrayAt(tokens, *index);

  if (strcmp(token, "(") == 0) {
    (*index)++;
    root = queryBuildTree(tokens, index, 0); 
    (*index)++;
  } else if (strcmp(token, "not") == 0) {
    (*index)++;
    queryTree *notNode = newNode("not");
    notNode->right = parseTerm(tokens, index);
    root = notNode;
  } else {
    root = newNode(token);
    (*index)++;
  }

  return root;
}

queryTree *queryBuildTree(darray *tokens, int *index, int precedence) {
  queryTree *left = parseTerm(tokens, index);

  while (*index < tokens->size) {
    char *token = darrayAt(tokens, *index);

    int currentPrecedence;
    if (strcmp(token, "or") == 0) {
      currentPrecedence = 1;
    } else if (strcmp(token, "and") == 0) {
      currentPrecedence = 2;
    } else {
      break;
    }

    if (currentPrecedence < precedence) {
      break;
    }

    (*index)++;
    queryTree *right = queryBuildTree(tokens, index, currentPrecedence);

    queryTree *operatorNode = newNode(token);
    operatorNode->left = left;
    operatorNode->right = right;
    left = operatorNode;
  }

  return left;
}

void printTreeHelper(queryTree *root, char *prefix, int isLast) {
  if (root == NULL) {
    return;
  }

  printf("%s", prefix);
  printf(isLast ? "└── " : "├── ");
  printf("%s\n", root->value);

  char newPrefix[256];
  snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLast ? "    " : "│   ");

  if (root->left != NULL || root->right != NULL) {
    printTreeHelper(root->left, newPrefix, root->right == NULL);
    printTreeHelper(root->right, newPrefix, 1);
  }
}

void queryTreePrint(queryTree *root) {
  printTreeHelper(root, "", 1);
}

