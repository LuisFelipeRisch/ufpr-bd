#include "conflictSerializable.h"

int checkConflictSerializable(char ***matrix, Array *activeTrans, int startIndex, int endIndex)
{
  int **dependencyGraph, sourceIndex, destinyIndex, exitsCycle, i, j;

  dependencyGraph = initIntMatrixWith(0, activeTrans->used, activeTrans->used);
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

        sourceIndex = checkElemExistence(activeTrans, atoi(matrix[i][TRANSACTION_INDEX]));
        destinyIndex = checkElemExistence(activeTrans, atoi(matrix[j][TRANSACTION_INDEX]));

        dependencyGraph[sourceIndex][destinyIndex]++;
      }

    if (!strcmp(matrix[i][OPERATION_INDEX], WRITE))
      for (j = i + 1; j <= endIndex; j++)
      {
        if (!strcmp(matrix[j][TRANSACTION_INDEX], matrix[i][TRANSACTION_INDEX]) ||
            (strcmp(matrix[j][OPERATION_INDEX], WRITE) && strcmp(matrix[j][OPERATION_INDEX], READ)) ||
            strcmp(matrix[j][ATTRIBUTE_INDEX], matrix[i][ATTRIBUTE_INDEX]))
          continue;

        sourceIndex = checkElemExistence(activeTrans, atoi(matrix[i][TRANSACTION_INDEX]));
        destinyIndex = checkElemExistence(activeTrans, atoi(matrix[j][TRANSACTION_INDEX]));

        dependencyGraph[sourceIndex][destinyIndex]++;
      }
  }

  exitsCycle = cycle(dependencyGraph, activeTrans->used);

  if (dependencyGraph)
  {
    for (i = 0; i < activeTrans->used; i++)
      free(dependencyGraph[i]);
    free(dependencyGraph);
  }

  return exitsCycle == -1 ? -1 : !exitsCycle;
}