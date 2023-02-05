#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "conflictSerializable.h"
#include "equivalentView.h"
#include "khan.h"

int main()
{
  char ***matrix, **activeTrans;
  int *delimitedSchedules, delimitedSchedulesSize, linesQnt,
      startIndex, endIndex, cycle, equivalentView, activeTransCount, i, j, success;
  uint *dependencyGraphSize, **dependencyGraph;
  dependencyGraphSize = malloc(sizeof(uint));

  success = 1;

  matrix = readEntryFromStdin(&linesQnt);
  if (!matrix)
    success = 0;

  delimitedSchedules = delimitSchedules(matrix, linesQnt, &delimitedSchedulesSize);
  if (!delimitedSchedules)
    success = 0;

  startIndex = 0;
  for (i = 0; i < delimitedSchedulesSize && success; i++)
  {
    endIndex = delimitedSchedules[i];

    activeTrans = getActiveTransactions(matrix, &activeTransCount, startIndex, endIndex);
    if (!activeTrans)
    {
      success = 0;
      continue;
    }

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
    activeTrans = NULL;
  }

  if (matrix)
  {
    for (i = 0; i < linesQnt; i++)
      for (j = 0; i < COL; i++)
        free(matrix[i][j]);

    for (i = 0; i < linesQnt; i++)
      free(matrix[i]);

    free(matrix);
  }

  if (activeTrans)
  {
    for (i = 0; i < activeTransCount; i++)
      free(activeTrans[i]);

    free(activeTrans);
  }

  if (delimitedSchedules)
    free(delimitedSchedules);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}