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

int **initIntMatrixWith(int number, uint row, uint col)
{
  int **matrix = allocIntMatrix(row, col);

  if (!matrix)
    return NULL;

  for (uint i = 0; i < row; i++)
    for (uint j = 0; j < col; j++)
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

Array *indexFinishedScaling(char ***matrix, uint row)
{
  int indexOfElem;
  uint currentTransaction;
  Array *activeTransactions = initArray(START_SIZE);
  if (!activeTransactions)
    return NULL;

  Array *indexedScaling = initArray(START_SIZE);
  if (!indexedScaling)
  {
    freeArray(activeTransactions);
    return NULL;
  }

  for (uint i = 0; i < row; i++)
  {
    currentTransaction = atoi(matrix[i][TRANSACTION_INDEX]);
    indexOfElem = checkElemExistence(activeTransactions, currentTransaction);

    if (indexOfElem == -1 && strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      push(activeTransactions, currentTransaction);
    else if (indexOfElem != -1 && !strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      activeTransactions = removeElem(activeTransactions, indexOfElem);

    if (isEmptyArray(activeTransactions))
      push(indexedScaling, i);
  }

  free(activeTransactions);
  return indexedScaling;
}

void getTimestampsSchedule(char ***matrix, Array *timestamps, uint startIndex, uint endIndex)
{
  uint i;

  for (i = startIndex; i <= endIndex; i++)
    if (strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      push(timestamps, atoi(matrix[i][TIME_INDEX]));
}

int getUniqAttributesSchedule(char ***matrix, char **attributes, int *attributesCount, uint startIndex, uint endIndex)
{
  uint i, j;
  int attrExists;

  for (i = startIndex; i <= endIndex; i++)
  {
    if (!strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      continue;

    attrExists = 0;
    for (j = 0; j < *attributesCount && !attrExists; j++)
      if (!strcmp(attributes[j], matrix[i][ATTRIBUTE_INDEX]))
        attrExists = 1;

    if (!attrExists)
    {
      attributes[*attributesCount] = calloc(strlen(matrix[i][ATTRIBUTE_INDEX]), sizeof(char));
      if (!attributes[*attributesCount])
        return -1;

      memcpy(attributes[*attributesCount], matrix[i][ATTRIBUTE_INDEX], strlen(matrix[i][ATTRIBUTE_INDEX]));
      (*attributesCount)++;
    }
  }

  return 1;
}

int getActiveTransactions(char ***matrix, char **activeTrans, int *activeTransCount, uint startIndex, uint endIndex)
{
  uint i, j;
  int transExits;

  for (i = startIndex; i <= endIndex; i++)
  {
    transExits = 0;
    for (j = 0; j < *activeTransCount && !transExits; j++)
      if (!strcmp(activeTrans[j], matrix[i][TRANSACTION_INDEX]))
        transExits = 1;

    if (!transExits)
    {
      activeTrans[*activeTransCount] = calloc(strlen(matrix[i][TRANSACTION_INDEX]), sizeof(char));
      if (!activeTrans[*activeTransCount])
        return -1;

      memcpy(activeTrans[*activeTransCount], matrix[i][TRANSACTION_INDEX], strlen(matrix[i][TRANSACTION_INDEX]));
      (*activeTransCount)++;
    }
  }

  return 1;
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

char ***buildNewMatrixWithInitialFinalTrans(char ***matrix, int *newMatrixSize, uint startIndex, uint endIndex)
{
  char **scheduleAttrs, ***newMatrix, line[256], *splittedLine;
  int splitsCount, maxBuffer, i, j, scheduleAttrsCount;

  newMatrix = malloc(sizeof(char **));
  if (!newMatrix)
    return NULL;

  scheduleAttrsCount = 0;
  scheduleAttrs = malloc(sizeof(char *));
  if (!scheduleAttrs)
    return NULL;

  getUniqAttributesSchedule(matrix, scheduleAttrs, &scheduleAttrsCount, startIndex, endIndex);

  *newMatrixSize = 0;
  i = 0;
  while (i < scheduleAttrsCount)
  {
    maxBuffer = sizeof("0") + sizeof(INITIAL_TRANS_IDENTIFIER) + sizeof(WRITE) + sizeof(scheduleAttrs[i]) + 1;
    snprintf(line, maxBuffer, "%s %s %s %s", "0", INITIAL_TRANS_IDENTIFIER, WRITE, scheduleAttrs[i]);

    newMatrix[*newMatrixSize] = calloc(COL, sizeof(char *));
    if (!newMatrix[*newMatrixSize])
      return NULL;

    splittedLine = strtok(line, " ");
    splitsCount = 0;
    while (splittedLine != NULL && splitsCount >= 0 && splitsCount <= 3)
    {
      newMatrix[*newMatrixSize][splitsCount] = calloc(sizeof(splittedLine), sizeof(char));
      if (!newMatrix[*newMatrixSize][splitsCount])
        return NULL;

      memcpy(newMatrix[*newMatrixSize][splitsCount], splittedLine, sizeof(splittedLine));
      splittedLine = strtok(NULL, " ");
      splitsCount++;
    }

    free(splittedLine);
    (*newMatrixSize)++;
    i++;
  }

  for (i = startIndex; i <= endIndex; i++)
  {
    if (!strcmp(matrix[i][OPERATION_INDEX], COMMIT))
      continue;

    newMatrix[*newMatrixSize] = calloc(COL, sizeof(char *));
    if (!newMatrix[*newMatrixSize])
      return NULL;

    for (j = 0; j < COL; j++)
    {
      newMatrix[*newMatrixSize][j] = calloc(sizeof(matrix[i][j]), sizeof(char));
      if (!newMatrix)
        return NULL;

      memcpy(newMatrix[*newMatrixSize][j], matrix[i][j], sizeof(matrix[i][j]));
    }

    (*newMatrixSize)++;
  }

  i = 0;
  while (i < scheduleAttrsCount)
  {
    maxBuffer = sizeof("0") + sizeof(FINAL_TRANS_IDENTIFIER) + sizeof(READ) + sizeof(scheduleAttrs[i]) + 1;
    snprintf(line, maxBuffer, "%s %s %s %s", "0", FINAL_TRANS_IDENTIFIER, READ, scheduleAttrs[i]);

    newMatrix[*newMatrixSize] = calloc(COL, sizeof(char *));
    if (!newMatrix[*newMatrixSize])
      return NULL;

    splittedLine = strtok(line, " ");
    splitsCount = 0;
    while (splittedLine != NULL && splitsCount >= 0 && splitsCount <= 3)
    {
      newMatrix[*newMatrixSize][splitsCount] = calloc(sizeof(splittedLine), sizeof(char));
      if (!newMatrix[*newMatrixSize][splitsCount])
        return NULL;

      memcpy(newMatrix[*newMatrixSize][splitsCount], splittedLine, sizeof(splittedLine));
      splittedLine = strtok(NULL, " ");
      splitsCount++;
    }

    free(splittedLine);
    (*newMatrixSize)++;
    i++;
  }

  return newMatrix;
}

char ***readEntryFromStdin(int *linesQnt)
{
  int i, j, splitsCount;
  char ***matrix, *splittedLine, line[256];

  *linesQnt = 0;
  matrix = malloc(sizeof(char **));
  if (!matrix)
    return NULL;

  while (fgets(line, sizeof(line), stdin))
  {
    matrix[*linesQnt] = calloc(COL, sizeof(char *));
    if (!matrix)
    {
      for (i = *linesQnt - 1; i >= 0; i--)
        free(matrix[i]);
      free(matrix);

      return NULL;
    }

    line[strcspn(line, "\n")] = 0;
    splittedLine = strtok(line, " ");
    splitsCount = 0;

    while (splittedLine != NULL && splitsCount >= 0 && splitsCount <= 3)
    {
      matrix[*linesQnt][splitsCount] = calloc(sizeof(splittedLine), sizeof(char));
      if (!matrix[*linesQnt][splitsCount])
      {
        for (i = *linesQnt; i >= 0; i--)
          for (j = splitsCount - 1; j >= 0; j--)
            free(matrix[i][j]);

        for (i = *linesQnt; i >= 0; i--)
          free(matrix[i]);

        free(matrix);

        return NULL;
      }

      memcpy(matrix[*linesQnt][splitsCount], splittedLine, sizeof(splittedLine));

      splittedLine = strtok(NULL, " ");
      splitsCount++;
    }

    (*linesQnt)++;
  }

  return matrix;
}