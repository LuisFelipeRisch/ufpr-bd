#include "utils.h"

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

int *initIntArrayWith(int number, uint size){
  int *array = allocIntArray(size);

  if(!array)
    return NULL;

  memset(array, number, size * sizeof(int));

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