#include "conflictSerializable.h"

int checkConflictSerializable(char ***matrix, char **activeTrans, int activeTransCount, int startIndex, int endIndex)
{
  int **dependencyGraph, sourceIndex, destinyIndex, exitsCycle, i, j;

  dependencyGraph = initIntMatrixWith(0, activeTransCount, activeTransCount);
  if (!dependencyGraph)
    return -1;

  for (i = startIndex; i <= endIndex - 1; i++)
  {
    if (strcmp(matrix[i][OPERATION_INDEX], READ) && strcmp(matrix[i][OPERATION_INDEX], WRITE))
      continue;

    if (!strcmp(matrix[i][OPERATION_INDEX], READ))
      for (j = i + 1; j <= endIndex; j++)
      {
        if (!strcmp(matrix[j][TRANSACTION_INDEX], matrix[i][TRANSACTION_INDEX]) ||
            strcmp(matrix[j][OPERATION_INDEX], WRITE) ||
            strcmp(matrix[j][ATTRIBUTE_INDEX], matrix[i][ATTRIBUTE_INDEX]))
          continue;

        sourceIndex = getIndexOfValue(matrix[i][TRANSACTION_INDEX], activeTrans, activeTransCount);
        destinyIndex = getIndexOfValue(matrix[j][TRANSACTION_INDEX], activeTrans, activeTransCount);

        dependencyGraph[sourceIndex][destinyIndex]++;
      }

    if (!strcmp(matrix[i][OPERATION_INDEX], WRITE))
      for (j = i + 1; j <= endIndex; j++)
      {
        if (!strcmp(matrix[j][TRANSACTION_INDEX], matrix[i][TRANSACTION_INDEX]) ||
            (strcmp(matrix[j][OPERATION_INDEX], WRITE) && strcmp(matrix[j][OPERATION_INDEX], READ)) ||
            strcmp(matrix[j][ATTRIBUTE_INDEX], matrix[i][ATTRIBUTE_INDEX]))
          continue;

        sourceIndex = getIndexOfValue(matrix[i][TRANSACTION_INDEX], activeTrans, activeTransCount);
        destinyIndex = getIndexOfValue(matrix[j][TRANSACTION_INDEX], activeTrans, activeTransCount);

        dependencyGraph[sourceIndex][destinyIndex]++;
      }
  }

  exitsCycle = cycle(dependencyGraph, activeTransCount);

  if (dependencyGraph)
  {
    for (i = 0; i < activeTransCount; i++)
      free(dependencyGraph[i]);
    free(dependencyGraph);
  }

  return exitsCycle == -1 ? -1 : !exitsCycle;
}