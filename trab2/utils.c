#include "utils.h"

uint *allocUintArray(uint size)
{
  uint *array = calloc(size, sizeof(uint));

  if (!array)
    return NULL;

  return array;
}

int *allocIntArray(uint size)
{
  int *array = calloc(size, sizeof(int));

  if (!array)
    return NULL;

  return array;
}

int **allocIntMatrix(uint row, uint col)
{
  int **matrix = calloc(row, sizeof(int *));

  if (!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
  {
    matrix[i] = allocIntArray(col);

    if (!matrix[i])
    {
      for (uint j = i; j >= 0; j--)
        free(matrix[j]);
      free(matrix);
      return NULL;
    }
  }

  return matrix;
}

uint **allocUintMatrix(uint row, uint col)
{
  uint **matrix = calloc(row, sizeof(uint *));

  if (!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
  {
    matrix[i] = allocUintArray(col);

    if (!matrix[i])
    {
      for (uint j = i; j >= 0; j--)
        free(matrix[j]);
      free(matrix);
      return NULL;
    }
  }

  return matrix;
}

int *initIntArrayWith(int number, uint size)
{
  int *array = allocIntArray(size);

  if (!array)
    return NULL;

  memset(array, number, size * sizeof(int));

  return array;
}

uint *initUintArrayWith(uint number, uint size)
{
  uint *array = allocUintArray(size);

  if (!array)
    return NULL;

  for (uint i = 0; i < size; i++)
    array[i] = number;

  return array;
}

int **initIntMatrixWith(int number, int row, int col)
{
  int i, j;
  int **matrix = allocIntMatrix(row, col);

  if (!matrix)
    return NULL;

  for (i = 0; i < row; i++)
    for (j = 0; j < col; j++)
      matrix[i][j] = number;

  return matrix;
}

uint **initUintMatrixWith(uint number, uint row, uint col)
{
  uint **matrix = allocUintMatrix(row, col);

  if (!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
    for (uint j = 0; j < col; j++)
      matrix[i][j] = number;

  return matrix;
}

Array *delimitSchedules(char ***matrix, int linesQnt)
{
  Array *activeTrans, *delimitedSchedules;
  int currentTrans, indexOfExistantTrans, i, success;

  success = 1;

  activeTrans = initArray(START_SIZE);
  if (!activeTrans)
    success = 0;

  delimitedSchedules = initArray(START_SIZE);
  if (!delimitedSchedules)
    success = 0;

  for (i = 0; i < linesQnt && success; i++)
  {
    currentTrans = atoi(matrix[i][TRANSACTION_INDEX]);
    indexOfExistantTrans = checkElemExistence(activeTrans, currentTrans);

    if (indexOfExistantTrans == -1 && strcmp(matrix[i][OPERATION_INDEX], COMMIT))
    {
      if (push(activeTrans, currentTrans) == -1)
      {
        success = 0;
        continue;
      }
    }
    else if (indexOfExistantTrans != -1 && !strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      activeTrans = removeElem(activeTrans, indexOfExistantTrans);

    if (isEmptyArray(activeTrans))
      if (push(delimitedSchedules, i) == -1)
      {
        success = 0;
        continue;
      }
  }

  if (activeTrans)
    freeArray(activeTrans);

  if (!success)
    if (delimitedSchedules)
      freeArray(delimitedSchedules);

  return success ? delimitedSchedules : NULL;
}

void getTimestampsSchedule(char ***matrix, Array *timestamps, uint startIndex, uint endIndex)
{
  uint i;

  for (i = startIndex; i <= endIndex; i++)
    if (strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      push(timestamps, atoi(matrix[i][TIME_INDEX]));
}

char **getUniqAttributesSchedule(char ***matrix, int *scheduleAttrsCount, int startIndex, int endIndex)
{
  char **scheduleAttrs;
  int i, j, attrExists, success, maxScheduleAttrsCount;

  success = 1;

  maxScheduleAttrsCount = endIndex - startIndex + 1;
  *scheduleAttrsCount = 0;
  scheduleAttrs = (char **)calloc(maxScheduleAttrsCount, sizeof(char *));
  if (!scheduleAttrs)
    success = 0;

  for (i = startIndex; i <= endIndex && success; i++)
  {
    if (!strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      continue;

    attrExists = 0;
    for (j = 0; j < *scheduleAttrsCount && !attrExists; j++)
      if (!strcmp(scheduleAttrs[j], matrix[i][ATTRIBUTE_INDEX]))
        attrExists = 1;

    if (!attrExists)
    {
      scheduleAttrs[*scheduleAttrsCount] = calloc(sizeof(matrix[i][ATTRIBUTE_INDEX]), sizeof(char));
      if (!scheduleAttrs[*scheduleAttrsCount])
      {
        success = 0;
        continue;
      }

      memcpy(scheduleAttrs[*scheduleAttrsCount], matrix[i][ATTRIBUTE_INDEX], sizeof(matrix[i][ATTRIBUTE_INDEX]));
      (*scheduleAttrsCount)++;
    }
  }

  if (!success)
    if (scheduleAttrs)
    {
      for (i = 0; i < maxScheduleAttrsCount; i++)
        if (scheduleAttrs[i])
          free(scheduleAttrs[i]);

      free(scheduleAttrs);
    }

  return success ? scheduleAttrs : NULL;
}

Array *getActiveTransactions(char ***matrix, int startIndex, int endIndex)
{
  Array *activeTrans;
  int currentTrans, i, j, indexOfExistantElem, success;

  success = 1;

  activeTrans = initArray(START_SIZE);
  if (!activeTrans)
    success = 0;

  for (i = startIndex; i <= endIndex && success; i++)
  {
    currentTrans = atoi(matrix[i][TRANSACTION_INDEX]);

    indexOfExistantElem = checkElemExistence(activeTrans, currentTrans);

    if (indexOfExistantElem == -1)
      if (push(activeTrans, currentTrans) == -1)
        success = 0;
  }

  if (!success)
    if (activeTrans)
      freeArray(activeTrans);

  return success ? activeTrans : NULL;
}

int getIndexOfValue(char *value, char **matrix, int matrixSize)
{
  int i, found;

  found = -1;

  for (i = 0; i < matrixSize && found == -1; i++)
    if (!strcmp(matrix[i], value))
      found = i;

  return found;
}

char ***buildNewMatrixWithInitialFinalTrans(char ***matrix, int *newMatrixSize, int activeTransCount, int startIndex, int endIndex)
{
  char **scheduleAttrs, ***newMatrix, line[256], *splittedLine;
  int splitsCount, maxBuffer, i, j, scheduleAttrsCount, counter, success;

  success = 1;

  scheduleAttrs = getUniqAttributesSchedule(matrix, &scheduleAttrsCount, startIndex, endIndex);
  if (!scheduleAttrs)
    success = 0;

  *newMatrixSize = endIndex - startIndex + 1 + scheduleAttrsCount * 2 - activeTransCount;
  newMatrix = (char ***)calloc(*newMatrixSize, sizeof(char **));
  if (!newMatrix)
    success = 0;

  if (success)
    for (i = 0; i < *newMatrixSize && success; i++)
    {
      newMatrix[i] = (char **)calloc(COL, sizeof(char *));
      if (!newMatrix[i])
        success = 0;
    }

  counter = 0;
  i = 0;
  while (i < scheduleAttrsCount && success)
  {
    maxBuffer = sizeof("0") + sizeof(INITIAL_TRANS_IDENTIFIER) + sizeof(WRITE) + sizeof(scheduleAttrs[i]);
    snprintf(line, maxBuffer, "%s %s %s %s", "0", INITIAL_TRANS_IDENTIFIER, WRITE, scheduleAttrs[i]);

    splittedLine = strtok(line, " ");
    splitsCount = 0;
    while (splittedLine != NULL && splitsCount >= 0 && splitsCount <= 3 && success)
    {
      newMatrix[counter][splitsCount] = (char *)calloc(sizeof(splittedLine), sizeof(char));
      if (!newMatrix[counter][splitsCount])
      {
        success = 0;
        continue;
      }

      memcpy(newMatrix[counter][splitsCount], splittedLine, sizeof(splittedLine));
      splittedLine = strtok(NULL, " ");
      splitsCount++;
    }

    free(splittedLine);
    counter++;
    i++;
  }

  for (i = startIndex; i <= endIndex && success; i++)
  {
    if (!strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      continue;

    for (j = 0; j < COL && success; j++)
    {
      newMatrix[counter][j] = (char *)calloc(sizeof(matrix[i][j]), sizeof(char));
      if (!newMatrix[counter][j])
      {
        success = 0;
        continue;
      }

      memcpy(newMatrix[counter][j], matrix[i][j], sizeof(matrix[i][j]));
    }

    (counter)++;
  }

  i = 0;
  while (i < scheduleAttrsCount && success)
  {
    maxBuffer = sizeof("0") + sizeof(FINAL_TRANS_IDENTIFIER) + sizeof(READ) + sizeof(scheduleAttrs[i]);
    snprintf(line, maxBuffer, "%s %s %s %s", "0", FINAL_TRANS_IDENTIFIER, READ, scheduleAttrs[i]);

    splittedLine = strtok(line, " ");
    splitsCount = 0;
    while (splittedLine != NULL && splitsCount >= 0 && splitsCount <= 3 && success)
    {
      newMatrix[counter][splitsCount] = (char *)calloc(sizeof(splittedLine), sizeof(char));
      if (!newMatrix[counter][splitsCount])
      {
        success = 0;
        continue;
      }

      memcpy(newMatrix[counter][splitsCount], splittedLine, sizeof(splittedLine));
      splittedLine = strtok(NULL, " ");
      splitsCount++;
    }

    free(splittedLine);
    (counter)++;
    i++;
  }

  if (scheduleAttrs)
  {
    int maxScheduleAttrsCount = endIndex - startIndex + 1;

    for (i = 0; i < maxScheduleAttrsCount; i++)
      if (scheduleAttrs[i])
        free(scheduleAttrs[i]);

    free(scheduleAttrs);
  }

  if (!success)
    if (newMatrix)
    {
      for (i = 0; i < *newMatrixSize; i++)
      {
        for (j = 0; j < COL; j++)
          if (newMatrix[i][j])
            free(newMatrix[i][j]);

        if (newMatrix[i])
          free(newMatrix[i]);
      }

      free(newMatrix);
    }

  return success ? newMatrix : NULL;
}

int countLinesOfStdinFile()
{
  int counter;
  char singleChar;

  counter = 0;

  while (fread(&singleChar, 1, 1, stdin))
    if (singleChar == '\n')
      counter++;

  return counter;
}

char ***readEntryFromStdin(int *linesQnt)
{
  int i, j, splitsCount, counter, success;
  char ***matrix, *splittedLine, line[256];

  success = 1;

  *linesQnt = countLinesOfStdinFile() + 1;
  rewind(stdin);
  matrix = (char ***)calloc(*linesQnt, sizeof(char **));
  if (!matrix)
    success = 0;

  for (i = 0; i < *linesQnt && success; i++)
  {
    matrix[i] = (char **)calloc(COL, sizeof(char *));
    if (!matrix[i])
      success = 0;
  }

  counter = 0;
  while (fgets(line, sizeof(line), stdin) && success)
  {
    line[strcspn(line, "\n")] = 0;
    splittedLine = strtok(line, " ");
    splitsCount = 0;

    while (splittedLine != NULL && splitsCount >= 0 && splitsCount <= 3 && success)
    {
      matrix[counter][splitsCount] = (char *)calloc(sizeof(splittedLine), sizeof(char));
      if (!matrix[counter][splitsCount])
      {
        success = 0;
        continue;
      }

      memcpy(matrix[counter][splitsCount], splittedLine, sizeof(splittedLine));
      splittedLine = strtok(NULL, " ");
      splitsCount++;
    }

    counter++;
  }

  if (!success)
    if (matrix)
    {
      for (i = 0; i < *linesQnt; i++)
      {
        for (j = 0; j < COL; j++)
          if (matrix[i][j])
            free(matrix[i][j]);

        if (matrix[i])
          free(matrix[i]);
      }

      free(matrix);
    }

  return success ? matrix : NULL;
}