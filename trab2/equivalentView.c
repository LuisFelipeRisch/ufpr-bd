#include "equivalentView.h"

int combineAndCheckCycle(uint **polygraph, int possibilityOne[][2], int possibilityTwo[][2], int *possibilities, int *data, int polygraphSize, int start, int end, int index, int k)
{
  if (index == k)
  {
    Queue *queue;
    int sourceIndex, destinyIndex, cycle;
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
      polygraph[sourceIndex][destinyIndex]++;
    }

    queue = initQueue();
    cycle = khan(polygraph, polygraphSize, queue);
    cleanQueue(queue);

    if (cycle)
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

int checkCycle(uint **polygraph, int possibilityOne[][2], int possibilityTwo[][2], int polygraphSize, int possibilityCount)
{
  int possibilities[] = {1, 2};
  int k = possibilityCount;
  int n = 2;
  int data[k];
  return combineAndCheckCycle(polygraph, possibilityOne, possibilityTwo, possibilities, data, polygraphSize, 0, n - 1, 0, k);
}

int checkEquivalencyView(char ***matrix, char **activeTrans, int activeTransCount, uint startIndex, uint endIndex)
{
  Queue *queue;
  int sourceIndex, destinyIndex, auxSourceIndex, auxDestinyIndex, auxSourceIndexTwo, auxDestinyIndexTwo, cycle, equivalent, i, j, k,
      newMatrixSize, found, possibilityOne[100][2], possibilityTwo[100][2], countPossibility;
  uint **polygraph;
  char ***newMatrix;

  const int INITIAL_TRANS_INDEX = 0;
  const int FINAL_TRANS_INDEX = activeTransCount + 1;

  polygraph = initUintMatrixWith(0, activeTransCount + 2, activeTransCount + 2);
  if (!polygraph)
    return -1;

  newMatrix = buildNewMatrixWithInitialFinalTrans(matrix, &newMatrixSize, startIndex, endIndex);
  if (!newMatrix)
    return -1;

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

    sourceIndex = getIndexOfValue(newMatrix[j + 1][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;
    destinyIndex = !strcmp(newMatrix[i][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? INITIAL_TRANS_INDEX : getIndexOfValue(newMatrix[i][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;
    polygraph[sourceIndex][destinyIndex]++;

    k = i + 1;
    while (k < newMatrixSize)
    {
      if (!strcmp(newMatrix[k][OPERATION_INDEX], WRITE) &&
          !strcmp(newMatrix[i][ATTRIBUTE_INDEX], newMatrix[j + 1][ATTRIBUTE_INDEX]) &&
          !strcmp(newMatrix[i][ATTRIBUTE_INDEX], newMatrix[k][ATTRIBUTE_INDEX]) &&
          strcmp(newMatrix[i][TRANSACTION_INDEX], newMatrix[j + 1][TRANSACTION_INDEX]) &&
          strcmp(newMatrix[i][TRANSACTION_INDEX], newMatrix[k][TRANSACTION_INDEX]) &&
          strcmp(newMatrix[j + 1][TRANSACTION_INDEX], newMatrix[k][TRANSACTION_INDEX]))
      {
        auxSourceIndex = getIndexOfValue(newMatrix[k][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;
        auxDestinyIndex = !strcmp(newMatrix[j + 1][TRANSACTION_INDEX], INITIAL_TRANS_IDENTIFIER) ? -1 : getIndexOfValue(newMatrix[j + 1][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;

        auxSourceIndexTwo = !strcmp(newMatrix[i][TRANSACTION_INDEX], FINAL_TRANS_IDENTIFIER) ? -1 : getIndexOfValue(newMatrix[i][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;
        auxDestinyIndexTwo = getIndexOfValue(newMatrix[k][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;

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
  equivalent = !checkCycle(polygraph, possibilityOne, possibilityTwo, activeTransCount + 2, countPossibility);

  return equivalent;
}