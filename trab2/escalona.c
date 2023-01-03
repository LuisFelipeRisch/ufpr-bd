#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "conflictSerializable.h"
#include "khan.h"

int main(){
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
      splittedLine = strtok (NULL, " ");
      splitsCount++;
    }

    free(splittedLine);
    row++;
  }

  Array *indexedScaling = indexFinishedScaling(matrix, row);
  uint startIndex = 0;
  for (uint i = 0; i < indexedScaling->used; i++){
    dependencyGraph = buildDependencyGraph(matrix, startIndex, indexedScaling->array[i], dependencyGraphSize);
    Queue *queue = initQueue();

    if(khan(dependencyGraph, *dependencyGraphSize, queue))
      printf("cicle");
    else
      printf("not cicle");

    startIndex = indexedScaling->array[i] + 1;
  }

  return 0;
}