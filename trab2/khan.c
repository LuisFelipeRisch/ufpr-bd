#include "khan.h"

int *buildIndegreeArray(uint **dependencyGraph, uint dependencyGraphSize){
  int *indegreeArray = initIntArrayWith(0, dependencyGraphSize);
  if(!indegreeArray)
    return NULL;

  for (uint j = 0; j < dependencyGraphSize; j++)
    for (uint i = 0; i < dependencyGraphSize; i++)
      indegreeArray[j] += dependencyGraph[i][j];

  return indegreeArray;
}

void enqueueWhenZero(Queue *queue, int *indegreeArray, uint dependencyGraphSize){
  for (uint i = 0; i < dependencyGraphSize; i++)
    if(!indegreeArray[i])
    {
      enqueue(queue, i);
      indegreeArray[i] = -1;
    }
}

int isThereCycle(int *indegreeArray, uint dependencyGraphSize){
  for (uint i = 0; i < dependencyGraphSize; i++)
    if(indegreeArray[i] > 0)
      return 1;

  return 0;
}

int khan(uint **dependencyGraph, uint dependencyGraphSize, Queue *serializedTransactions){
  int *indegreeArray = buildIndegreeArray(dependencyGraph, dependencyGraphSize);
  if(!indegreeArray)
    return 0;

  Queue *queue = initQueue();
  if(!queue)
    return 0;

  enqueueWhenZero(queue, indegreeArray, dependencyGraphSize);

  int transactionIndex;
  while (!isEmpty(queue))
  {
    transactionIndex = dequeue(queue);
    for (uint j = 0; j < dependencyGraphSize; j++)
      indegreeArray[j] -= dependencyGraph[transactionIndex][j];

    enqueue(serializedTransactions, transactionIndex);
    enqueueWhenZero(queue, indegreeArray, dependencyGraphSize);
  }

  return isThereCycle(indegreeArray, dependencyGraphSize);
}