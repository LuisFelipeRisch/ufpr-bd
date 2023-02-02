#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "conflictSerializable.h"
#include "equivalentView.h"
#include "khan.h"

#define N 4

int main()
{
  char line[256];
  char *splittedLine = NULL;
  uint row, splitsCount, *dependencyGraphSize, **dependencyGraph;
  dependencyGraphSize = malloc(sizeof(uint));
  char ***matrix = malloc(sizeof(char **));

  row = 0;
  while (fgets(line, sizeof(line), stdin))
  {
    matrix[row] = calloc(COL, sizeof(char *));

    line[strcspn(line, "\n")] = 0;
    splittedLine = strtok(line, " ");
    splitsCount = 0;
    while (splittedLine != NULL && splitsCount >= 0 && splitsCount <= 3)
    {
      matrix[row][splitsCount] = calloc(strlen(splittedLine), sizeof(char));
      memcpy(matrix[row][splitsCount], splittedLine, strlen(splittedLine));
      splittedLine = strtok(NULL, " ");
      splitsCount++;
    }

    free(splittedLine);
    row++;
  }

  Array *indexedScaling = indexFinishedScaling(matrix, row);

  uint startIndex = 0, endIndex;
  int cycle, equivalentView, activeTransCount;
  char **activeTrans;

  for (uint i = 0; i < indexedScaling->used; i++)
  {
    endIndex = indexedScaling->array[i];
    activeTransCount = 0;
    activeTrans = malloc(sizeof(char *));
    if (!activeTrans)
      return -1;

    getActiveTransactions(matrix, activeTrans, &activeTransCount, startIndex, endIndex);

    dependencyGraph = buildDependencyGraph(matrix, startIndex, endIndex, dependencyGraphSize);

    Queue *queue = initQueue();
    cycle = khan(dependencyGraph, *dependencyGraphSize, queue);

    equivalentView = checkEquivalencyView(matrix, activeTrans, activeTransCount, startIndex, endIndex);

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

  // QuickPerm();

  return 0;
}