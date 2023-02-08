#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "conflictSerializable.h"
#include "equivalentView.h"
#include "khan.h"

int main()
{
  Array *delimitedSchedules, *activeTrans;
  char ***matrix;
  int linesQnt, startIndex, endIndex, conflictSerializable, equivalentView, i, j, success;

  success = 1;

  matrix = readEntryFromStdin(&linesQnt);
  if (!matrix)
    success = 0;

  delimitedSchedules = delimitSchedules(matrix, linesQnt);
  if (!delimitedSchedules)
    success = 0;

  startIndex = 0;
  for (i = 0; i < delimitedSchedules->used && success; i++)
  {
    endIndex = delimitedSchedules->array[i];

    activeTrans = getActiveTransactions(matrix, startIndex, endIndex);
    if (!activeTrans)
    {
      success = 0;
      continue;
    }

    conflictSerializable = checkConflictSerializable(matrix, activeTrans, startIndex, endIndex);
    if (conflictSerializable == -1)
    {
      success = 0;
      continue;
    }

    equivalentView = checkEquivalencyView(matrix, activeTrans, startIndex, endIndex);
    if (equivalentView == -1)
    {
      success = 0;
      continue;
    }

    printf("%d ", i + 1);
    for (j = 0; j < activeTrans->used; j++)
    {
      printf("%d", activeTrans->array[j]);
      if (j != activeTrans->used - 1)
        printf(",");
    }
    printf(" %s", conflictSerializable ? "SS" : "NS");
    printf(" %s\n", equivalentView ? "SV" : "NV");

    freeArray(activeTrans);
    activeTrans = NULL;

    startIndex = endIndex + 1;
  }

  if (matrix)
  {
    for (i = 0; i < linesQnt; i++)
    {
      for (j = 0; j < COL; j++)
        if (matrix[i][j])
          free(matrix[i][j]);

      if (matrix[i])
        free(matrix[i]);
    }

    free(matrix);
  }

  if (delimitedSchedules)
    freeArray(delimitedSchedules);

  if (activeTrans)
    freeArray(activeTrans);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}