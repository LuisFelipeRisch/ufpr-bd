#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "conflictSerializable.h"
#include "equivalentView.h"
#include "khan.h"

int main()
{
  char ***matrix;
  int *delimitedSchedules, delimitedSchedulesSize, linesQnt;
  uint *dependencyGraphSize, **dependencyGraph;
  dependencyGraphSize = malloc(sizeof(uint));

  matrix = readEntryFromStdin(&linesQnt);
  if (!matrix)
    return EXIT_FAILURE;

  delimitedSchedules = delimitSchedules(matrix, linesQnt, &delimitedSchedulesSize);
  if (!delimitedSchedules)
    return EXIT_FAILURE;

  uint startIndex = 0, endIndex;
  int cycle, equivalentView, activeTransCount;
  char **activeTrans;

  for (uint i = 0; i < delimitedSchedulesSize; i++)
  {
    endIndex = delimitedSchedules[i];
    activeTransCount = 0;
    activeTrans = malloc(sizeof(char *));
    if (!activeTrans)
      return -1;

    getActiveTransactions(matrix, activeTrans, &activeTransCount, startIndex, endIndex);

    dependencyGraph = buildDependencyGraph(matrix, startIndex, endIndex, dependencyGraphSize);

    Queue *queue = initQueue();
    cycle = khan(dependencyGraph, *dependencyGraphSize, queue);

    // equivalentView = checkEquivalencyView(matrix, activeTrans, activeTransCount, startIndex, endIndex);

    printf("%d ", i + 1);
    for (uint j = 0; j < activeTransCount; j++)
    {
      printf("%s", activeTrans[j]);
      if (j != activeTransCount - 1)
        printf(",");
    }
    printf(" %s", cycle ? "NS" : "SS");
    printf(" %s\n", equivalentView ? "SV" : "NV");

    startIndex = endIndex + 1;
    free(activeTrans);
  }

  return 0;
}