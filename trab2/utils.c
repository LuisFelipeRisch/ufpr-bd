#include "utils.h"

uint *allocUintArray(uint size){
  uint *array = calloc(size, sizeof(uint));

  if(!array)
    return NULL;

  return array;
}

int *allocIntArray(uint size){
  int *array = calloc(size, sizeof(int));

  if(!array)
    return NULL;

  return array;
}

int **allocIntMatrix(uint row, uint col){
  int **matrix = calloc(row, sizeof(int *));

  if(!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
  {
    matrix[i] = allocIntArray(col);

    if(!matrix[i]){
      for (uint j = i; j >= 0; j--)
        free(matrix[j]);
      free(matrix);
      return NULL;
    }
  }

  return matrix;
}

uint **allocUintMatrix(uint row, uint col){
  uint **matrix = calloc(row, sizeof(uint *));

  if(!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
  {
    matrix[i] = allocUintArray(col);

    if(!matrix[i]){
      for (uint j = i; j >= 0; j--)
        free(matrix[j]);
      free(matrix);
      return NULL;
    }
  }

  return matrix;
}

int *initIntArrayWith(int number, uint size){
  int *array = allocIntArray(size);

  if(!array)
    return NULL;

  memset(array, number, size * sizeof(int));

  return array;
}

uint *initUintArrayWith(uint number, uint size){
  uint *array = allocUintArray(size);

  if(!array)
    return NULL;

  for (uint i = 0; i < size; i++)
    array[i] = number;

  return array;
}

int **initIntMatrixWith(int number, uint row, uint col){
  int **matrix = allocIntMatrix(row, col);

  if(!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
    for (uint j = 0; j < col; j++)
      matrix[i][j] = number;

  return matrix;
}

uint **initUintMatrixWith(uint number, uint row, uint col){
  uint **matrix = allocUintMatrix(row, col);

  if(!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
    for (uint j = 0; j < col; j++)
      matrix[i][j] = number;

  return matrix;
}

Array *indexFinishedScaling(char ***matrix, uint row){
  int indexOfElem;
  uint currentTransaction;
  Array *activeTransactions = initArray(START_SIZE);
  if(!activeTransactions)
    return NULL;

  Array *indexedScaling = initArray(START_SIZE);
  if(!indexedScaling){
    freeArray(activeTransactions);
    return NULL;
  }

  for (uint i = 0; i < row; i++)
  {
    currentTransaction = atoi(matrix[i][TRANSACTION_INDEX]);
    indexOfElem = checkElemExistence(activeTransactions, currentTransaction);

    if(indexOfElem == -1 && strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      push(activeTransactions, currentTransaction);
    else if(indexOfElem != -1 && !strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      activeTransactions = removeElem(activeTransactions, indexOfElem);

    if(isEmptyArray(activeTransactions))
      push(indexedScaling, i);
  }

  free(activeTransactions);
  return indexedScaling;
}