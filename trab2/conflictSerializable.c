#include "conflictSerializable.h"

uint findHigherTransactionNumber(char ***matrix, uint row){
  uint higher = atoi(matrix[0][TRANSACTION_INDEX]);

  for (uint i = 1; i < row; i++)
    if(atoi(matrix[i][TRANSACTION_INDEX]) > higher)
      higher = atoi(matrix[i][TRANSACTION_INDEX]);

  return higher;
}

uint **buildDependencyGraph(char ***matrix, uint row, uint *dependencyGraphSize){
  *dependencyGraphSize = findHigherTransactionNumber(matrix, row);
  uint **dependencyGraph = initUintMatrixWith(0, *dependencyGraphSize, *dependencyGraphSize);
  if(!dependencyGraph)
    return NULL;

  for (uint i = 0; i < row - 1; i++)
  {
    if(strcmp(matrix[i][OPERATION_INDEX], READ) && strcmp(matrix[i][OPERATION_INDEX], WRITE))
      continue;

    if(!strcmp(matrix[i][OPERATION_INDEX], READ))
      for (uint j = i + 1; j < row; j++)
      {
        if(!strcmp(matrix[j][TRANSACTION_INDEX], matrix[i][TRANSACTION_INDEX]) || strcmp(matrix[j][OPERATION_INDEX], WRITE) || strcmp(matrix[j][ATTRIBUTE_INDEX], matrix[i][ATTRIBUTE_INDEX]))
          continue;

        int transactionI = atoi(matrix[i][TRANSACTION_INDEX]) - 1;
        int transactionJ = atoi(matrix[j][TRANSACTION_INDEX]) - 1;

        dependencyGraph[transactionI][transactionJ]++;
      }

    if(!strcmp(matrix[i][OPERATION_INDEX], WRITE))
      for (uint j = i + 1; j < row; j++)
      {
        if(!strcmp(matrix[j][TRANSACTION_INDEX], matrix[i][TRANSACTION_INDEX]) || (strcmp(matrix[j][OPERATION_INDEX], WRITE) && strcmp(matrix[j][OPERATION_INDEX], READ)) || strcmp(matrix[j][ATTRIBUTE_INDEX], matrix[i][ATTRIBUTE_INDEX]))
          continue;

        int transactionI = atoi(matrix[i][TRANSACTION_INDEX]) - 1;
        int transactionJ = atoi(matrix[j][TRANSACTION_INDEX]) - 1;

        dependencyGraph[transactionI][transactionJ]++;
      }

  }

  return dependencyGraph;
}