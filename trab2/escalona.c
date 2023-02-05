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
      startIndex, endIndex, conflictSerializable, equivalentView, activeTransCount, i, j, success;

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

    conflictSerializable = checkConflictSerializable(matrix, activeTrans, activeTransCount, startIndex, endIndex);
    if (conflictSerializable == -1)
    {
      success = 0;
      continue;
    }

    equivalentView = checkEquivalencyView(matrix, activeTrans, activeTransCount, startIndex, endIndex);

    printf("%d ", i + 1);
    for (uint j = 0; j < activeTransCount; j++)
    {
      printf("%s", activeTrans[j]);
      if (j != activeTransCount - 1)
        printf(",");
    }
    printf(" %s", conflictSerializable ? "SS" : "NS");
    printf(" %s\n", equivalentView ? "SV" : "NV");

    startIndex = endIndex + 1;

    if (activeTrans)
    {
      for (j = 0; j < activeTransCount; j++)
        free(activeTrans[j]);

      free(activeTrans);
    }
    activeTrans = NULL;
  }

  if (matrix)
  {
    for (i = 0; i < linesQnt; i++)
      for (j = 0; i < COL; i++)
        free(matrix[i][j]);

    for (i = 0; i < linesQnt; i++)
      free(matrix[i]);
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