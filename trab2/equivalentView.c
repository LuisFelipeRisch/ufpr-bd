#include "equivalentView.h"

int checkEquivalencyView(char ***matrix, char **activeTrans, int activeTransCount, uint startIndex, uint endIndex)
{
  Queue *queue;
  int sourceIndex, destinyIndex, auxSourceIndex, auxDestinyIndex, cycle, equivalent, i, j, k, newMatrixSize, found;
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

  equivalent = 1;
  for (i = 0; i < newMatrixSize && equivalent; i++)
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
    while (k < newMatrixSize && equivalent)
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

        if (auxDestinyIndex != -1)
        {
          polygraph[auxSourceIndex][auxDestinyIndex]++;

          queue = initQueue();
          cycle = khan(polygraph, activeTransCount + 2, queue);
          cleanQueue(queue);

          if (!cycle)
          {
            k++;
            continue;
          }

          polygraph[auxSourceIndex][auxDestinyIndex]--;
        }

        auxSourceIndex = !strcmp(newMatrix[i][TRANSACTION_INDEX], FINAL_TRANS_IDENTIFIER) ? -1 : getIndexOfValue(newMatrix[i][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;
        auxDestinyIndex = getIndexOfValue(newMatrix[k][TRANSACTION_INDEX], activeTrans, activeTransCount) + 1;

        if (auxSourceIndex != -1)
        {
          polygraph[auxSourceIndex][auxDestinyIndex]++;

          queue = initQueue();
          cycle = khan(polygraph, activeTransCount + 2, queue);
          cleanQueue(queue);

          if (!cycle)
          {
            k++;
            continue;
          }

          polygraph[auxSourceIndex][auxDestinyIndex]--;
        }

        equivalent = 0;
      }
      k++;
    }
  }

  return equivalent;
}