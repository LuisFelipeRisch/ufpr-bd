#include "equivalentView.h"

int combineAndCheckCycle(int **polygraph, int possibilityOne[][2], int possibilityTwo[][2], int *possibilities, int *data, int polygraphSize, int start, int end, int index, int k)
{
  if (index == k)
  {
    int sourceIndex, destinyIndex, exitsCycle;
    for (int j = 0; j < k; j++)
    {
      printf("%d ", data[j]);
      if (data[j] == 1)
      {
        sourceIndex = possibilityOne[j][0];
        destinyIndex = possibilityOne[j][1];
      }
      else
      {
        sourceIndex = possibilityTwo[j][0];
        destinyIndex = possibilityTwo[j][1];
      }
      polygraph[sourceIndex][destinyIndex]++;
    }

    printf("\n");

    exitsCycle = cycle(polygraph, polygraphSize);

    if (exitsCycle)
      return 1;

    for (int j = 0; j < k; j++)
    {
      if (data[j] == 1)
      {
        sourceIndex = possibilityOne[j][0];
        destinyIndex = possibilityOne[j][1];
      }
      else
      {
        sourceIndex = possibilityTwo[j][0];
        destinyIndex = possibilityTwo[j][1];
      }
      polygraph[sourceIndex][destinyIndex]--;
    }

    return 0;
  }

  for (int i = start; i <= end; i++)
  {
    data[index] = possibilities[i];
    if (combineAndCheckCycle(polygraph, possibilityOne, possibilityTwo, possibilities, data, polygraphSize, start, end, index + 1, k))
      return 1;
  }

  return 0;
}

int checkCycle(int **polygraph, int possibilityOne[][2], int possibilityTwo[][2], int polygraphSize, int possibilityCount)
{
  int possibilities[] = {1, 2};
  int k = possibilityCount;
  int n = 2;
  int data[k];
  return combineAndCheckCycle(polygraph, possibilityOne, possibilityTwo, possibilities, data, polygraphSize, 0, n - 1, 0, k);
}

int checkEquivalencyView(char ***matrix, Array *activeTrans, int startIndex, int endIndex)
{
  int **polygraph, sourceIndex, destinyIndex, auxSourceIndex, auxDestinyIndex, auxSourceIndexTwo, auxDestinyIndexTwo, equivalent, i, j, k, actualJ,
      newMatrixSize, found, possibilityOne[100][2], possibilityTwo[100][2], countPossibility, success;
  char ***newMatrix;

  const int INITIAL_TRANS_INDEX = 0;
  const int FINAL_TRANS_INDEX = activeTrans->used + 1;

  polygraph = initIntMatrixWith(0, activeTrans->used + 2, activeTrans->used + 2);
  if (!polygraph)
    success = 0;

  newMatrix = buildNewMatrixWithInitialFinalTrans(matrix, &newMatrixSize, activeTrans->used, startIndex, endIndex);
  if (!newMatrix)
    success = 0;

  countPossibility = 0;
  for (i = 0; i < newMatrixSize; i++)
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

    sourceIndex = !strcmp(newMatrix[actualJ][TRANSACTION_INDEX], FINAL_TRANS_IDENTIFIER) ? FINAL_TRANS_INDEX : checkElemExistence(activeTrans, atoi(newMatrix[actualJ][TRANSACTION_INDEX])) + 1;
    destinyIndex = !strcmp(newMatrix[i][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? INITIAL_TRANS_INDEX : checkElemExistence(activeTrans, atoi(newMatrix[i][TRANSACTION_INDEX])) + 1;
    polygraph[sourceIndex][destinyIndex]++;

    k = i + 1;
    while (k < newMatrixSize)
    {
      if (!strcmp(newMatrix[k][OPERATION_INDEX], WRITE) &&
          !strcmp(newMatrix[i][ATTRIBUTE_INDEX], newMatrix[actualJ][ATTRIBUTE_INDEX]) &&
          !strcmp(newMatrix[i][ATTRIBUTE_INDEX], newMatrix[k][ATTRIBUTE_INDEX]) &&
          strcmp(newMatrix[i][TRANSACTION_INDEX], newMatrix[actualJ][TRANSACTION_INDEX]) &&
          strcmp(newMatrix[i][TRANSACTION_INDEX], newMatrix[k][TRANSACTION_INDEX]) &&
          strcmp(newMatrix[actualJ][TRANSACTION_INDEX], newMatrix[k][TRANSACTION_INDEX]))
      {
        auxSourceIndex = checkElemExistence(activeTrans, atoi(newMatrix[k][TRANSACTION_INDEX])) + 1;
        auxDestinyIndex = !strcmp(newMatrix[actualJ][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? -1 : checkElemExistence(activeTrans, atoi(newMatrix[actualJ][TRANSACTION_INDEX])) + 1;

        auxSourceIndexTwo = !strcmp(newMatrix[i][TRANSACTION_INDEX], FINAL_TRANS_IDENTIFIER) ? -1 : checkElemExistence(activeTrans, atoi(newMatrix[i][TRANSACTION_INDEX])) + 1;
        auxDestinyIndexTwo = checkElemExistence(activeTrans, atoi(newMatrix[k][TRANSACTION_INDEX])) + 1;

        if (auxDestinyIndex == -1 && auxSourceIndexTwo == -1)
        {
          k++;
          continue;
        }

        if (auxDestinyIndex == -1)
        {
          k++;

          polygraph[auxSourceIndexTwo][auxDestinyIndexTwo]++;
          continue;
        }

        if (auxSourceIndexTwo == -1)
        {
          k++;

          polygraph[auxSourceIndex][auxDestinyIndex]++;
          continue;
        }

        possibilityOne[countPossibility][0] = auxSourceIndex;
        possibilityOne[countPossibility][1] = auxDestinyIndex;

        possibilityTwo[countPossibility][0] = auxSourceIndexTwo;
        possibilityTwo[countPossibility][1] = auxDestinyIndexTwo;

        countPossibility++;
      }

      k++;
    }
  }

  equivalent = countPossibility > 0 ? !checkCycle(polygraph, possibilityOne, possibilityTwo, activeTrans->used + 2, countPossibility) : !cycle(polygraph, activeTrans->used + 2);

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

  return equivalent;
}