#include "equivalentView.h"

int isScheduleViewEquivalentRecursively(char ***matrix, int **currentPolyGraph, Array *activeTrans, int currentPolygraphSize, int startIndex, int matrixSize)
{
  int i, j, k, breakAll, sourceIndex, destinyIndex, sourceIndexTwo, destinyIndexTwo, success;

  success = 1;
  breakAll = 0;
  i = startIndex;
  while (i < matrixSize && !breakAll && success)
  {
    if (!strcmp(matrix[i][OPERATION_INDEX], WRITE))
    {
      j = i - 1;
      while (j >= 0 && !breakAll && success)
      {
        if (!strcmp(matrix[j][OPERATION_INDEX], READ) &&
            !strcmp(matrix[i][ATTRIBUTE_INDEX], matrix[j][ATTRIBUTE_INDEX]) &&
            strcmp(matrix[i][TRANSACTION_INDEX], matrix[j][TRANSACTION_INDEX]))
        {
          k = j - 1;
          while (k >= 0 && !breakAll && success)
          {
            if (!strcmp(matrix[k][OPERATION_INDEX], WRITE) &&
                !strcmp(matrix[i][ATTRIBUTE_INDEX], matrix[k][ATTRIBUTE_INDEX]) &&
                strcmp(matrix[i][TRANSACTION_INDEX], matrix[k][TRANSACTION_INDEX]) &&
                strcmp(matrix[j][TRANSACTION_INDEX], matrix[k][TRANSACTION_INDEX]))
            {
              sourceIndex = !strcmp(matrix[i][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? -1 : checkElemExistence(activeTrans, atoi(matrix[i][TRANSACTION_INDEX])) + 1;
              destinyIndex = !strcmp(matrix[k][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? -1 : checkElemExistence(activeTrans, atoi(matrix[k][TRANSACTION_INDEX])) + 1;

              sourceIndexTwo = !strcmp(matrix[j][TRANSACTION_INDEX], FINAL_TRANS_IDENTIFIER) ? -1 : checkElemExistence(activeTrans, atoi(matrix[j][TRANSACTION_INDEX])) + 1;
              destinyIndexTwo = !strcmp(matrix[i][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? -1 : checkElemExistence(activeTrans, atoi(matrix[i][TRANSACTION_INDEX])) + 1;

              if ((sourceIndex == -1 || destinyIndex == -1) && (sourceIndexTwo == -1 || destinyIndexTwo == -1))
                break;

              if ((sourceIndex == -1 || destinyIndex == -1))
              {
                currentPolyGraph[sourceIndexTwo][destinyIndexTwo]++;
                break;
              }

              if ((sourceIndexTwo == -1 || destinyIndexTwo == -1))
              {
                currentPolyGraph[sourceIndex][destinyIndex]++;
                break;
              }

              currentPolyGraph[sourceIndex][destinyIndex]++;
              switch (isScheduleViewEquivalentRecursively(matrix, currentPolyGraph, activeTrans, currentPolygraphSize, i + 1, matrixSize))
              {
              case 1:
                return 1;
                break;
              case -1:
                success = 0;
                break;
              default:
                break;
              }
              currentPolyGraph[sourceIndex][destinyIndex]--;

              if (!success)
                continue;

              currentPolyGraph[sourceIndexTwo][destinyIndexTwo]++;
              switch (isScheduleViewEquivalentRecursively(matrix, currentPolyGraph, activeTrans, currentPolygraphSize, i + 1, matrixSize))
              {
              case 1:
                return 1;
                break;
              case -1:
                success = 0;
                break;
              default:
                break;
              }
              currentPolyGraph[sourceIndexTwo][destinyIndexTwo]--;

              breakAll = 1;
            }
            k--;
          }
        }
        j--;
      }
    }

    if (!breakAll)
      i++;
  }

  if (!success)
    return -1;

  if (i == matrixSize)
    return !cycle(currentPolyGraph, currentPolygraphSize);

  return 0;
}

int checkEquivalencyView(char ***matrix, Array *activeTrans, int startIndex, int endIndex)
{
  int **polygraph, sourceIndex, destinyIndex, equivalent, i, j, actualJ, newMatrixSize, found, success;
  char ***newMatrix;

  const int INITIAL_TRANS_INDEX = 0;
  const int FINAL_TRANS_INDEX = activeTrans->used + 1;

  polygraph = initIntMatrixWith(0, activeTrans->used + 2, activeTrans->used + 2);
  if (!polygraph)
    success = 0;

  newMatrix = buildNewMatrixWithInitialFinalTrans(matrix, &newMatrixSize, activeTrans->used, startIndex, endIndex);
  if (!newMatrix)
    success = 0;

  for (i = 0; i < newMatrixSize && success; i++)
  {
    if (strcmp(newMatrix[i][OPERATION_INDEX], READ))
      continue;

    found = 0;
    j = i - 1;
    while (j >= 0 && !found)
    {
      if (!strcmp(newMatrix[j][OPERATION_INDEX], WRITE) &&
          !strcmp(newMatrix[i][ATTRIBUTE_INDEX], newMatrix[j][ATTRIBUTE_INDEX]) &&
          strcmp(newMatrix[i][TRANSACTION_INDEX], newMatrix[j][TRANSACTION_INDEX]))
        found = 1;
      j--;
    }

    if (!found)
      continue;

    actualJ = j + 1;

    sourceIndex = !strcmp(newMatrix[actualJ][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? INITIAL_TRANS_INDEX : checkElemExistence(activeTrans, atoi(newMatrix[actualJ][TRANSACTION_INDEX])) + 1;
    destinyIndex = !strcmp(newMatrix[i][TRANSACTION_INDEX], FINAL_TRANS_IDENTIFIER) ? FINAL_TRANS_INDEX : checkElemExistence(activeTrans, atoi(newMatrix[i][TRANSACTION_INDEX])) + 1;
    polygraph[sourceIndex][destinyIndex]++;
  }

  equivalent = isScheduleViewEquivalentRecursively(newMatrix, polygraph, activeTrans, activeTrans->used + 2, 0, newMatrixSize);

  if (equivalent == -1)
    success = 0;

  if (polygraph)
  {
    for (i = 0; i < activeTrans->used + 2; i++)
      if (polygraph[i])
        free(polygraph[i]);

    free(polygraph);
  }

  if (newMatrix)
  {
    for (i = 0; i < newMatrixSize; i++)
    {
      for (j = 0; j < COL; j++)
        if (newMatrix[i][j])
          free(newMatrix[i][j]);

      if (newMatrix[i])
        free(newMatrix[i]);
    }

    free(newMatrix);
  }

  return success ? equivalent : -1;
}