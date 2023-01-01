#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "conflictSerializable.h"

void sortMatrixLinesByTime(char ***matrix, uint row){
  uint i, j, minIdx;

  for (i = 0; i < row - 1; i++)
  {
    minIdx = i;

    for (j = i+1; j < row; j++)
      if(atoi(matrix[j][TIME_INDEX]) < atoi(matrix[minIdx][TIME_INDEX]))
        minIdx = j;

    if(minIdx != i)
    {
      char **row = matrix[i];
      matrix[i] = matrix[minIdx];
      matrix[minIdx] = row;
    }
  }
}

int main(){
  char line[256];
  char *splittedLine = NULL;
  uint row, splitsCount, *dependencyGraphSize;
  dependencyGraphSize = malloc(sizeof(uint));
  char ***matrix = malloc(sizeof(char **));
  int **dependencyGraph;

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

  sortMatrixLinesByTime(matrix, row);

  for (uint i = 0; i < row; i++)
  {
    for (uint j = 0; j < COL; j++)
    {
      printf("%s ", matrix[i][j]);
    }
    printf("\n");
  }

  dependencyGraph = buildDependencyGraph(matrix, row, dependencyGraphSize);

  for (uint i = 0; i < *dependencyGraphSize; i++)
  {
    for (uint j = 0; j < *dependencyGraphSize; j++)
    {
      printf("%d ", dependencyGraph[i][j]);
    }
    printf("\n");
  }

  return 0;
}