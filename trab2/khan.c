#include "khan.h"

int *buildIndegreeArray(int **directedGraph, int graphSize)
{
  int i, j, *indegreeArray;

  indegreeArray = initIntArrayWith(0, graphSize);
  if (!indegreeArray)
    return NULL;

  for (j = 0; j < graphSize; j++)
    for (i = 0; i < graphSize; i++)
      indegreeArray[j] += directedGraph[i][j];

  return indegreeArray;
}

void enqueueWhenZero(Queue *queue, int *indegreeArray, int graphSize)
{
  int i;
  for (i = 0; i < graphSize; i++)
    if (!indegreeArray[i])
    {
      enqueue(queue, i);
      indegreeArray[i] = -1;
    }
}

int isThereCycle(int *indegreeArray, int graphSize)
{
  for (int i = 0; i < graphSize; i++)
    if (indegreeArray[i] > 0)
      return 1;

  return 0;
}

int cycle(int **directedGraph, int directedGraphSize)
{
  Queue *queue;
  int *indegreeArray, success, i, transactionIndex, exitsCycle;

  success = 1;

  indegreeArray = buildIndegreeArray(directedGraph, directedGraphSize);
  if (!indegreeArray)
    success = 0;

  queue = initQueue();
  if (!queue)
    success = 0;

  if (success)
  {
    enqueueWhenZero(queue, indegreeArray, directedGraphSize);

    while (!isEmpty(queue))
    {
      transactionIndex = dequeue(queue);
      for (i = 0; i < directedGraphSize; i++)
        indegreeArray[i] -= directedGraph[transactionIndex][i];

      enqueueWhenZero(queue, indegreeArray, directedGraphSize);
    }

    exitsCycle = isThereCycle(indegreeArray, directedGraphSize);
  }

  if (queue)
    cleanQueue(queue);

  if (indegreeArray)
    free(indegreeArray);

  return success ? exitsCycle : -1;
}