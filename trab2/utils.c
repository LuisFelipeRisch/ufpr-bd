#include "utils.h"

uint *allocUintArray(uint size);
int *allocIntArray(uint size);
int **allocIntMatrix(uint row, uint col);
uint **allocUintMatrix(uint row, uint col);
int *initIntArrayWith(int number, uint size);
uint *initUintArrayWith(uint number, uint size);
int **initIntMatrixWith(int number, int row, int col);
uint **initUintMatrixWith(uint number, uint row, uint col);
int *delimitSchedules(char ***matrix, int linesQnt, int *indexedQnt);
void getTimestampsSchedule(char ***matrix, Array *timestamps, uint startIndex, uint endIndex);
int getUniqAttributesSchedule(char ***matrix, char **attributes, int *attributesCount, uint startIndex, uint endIndex);
char **getActiveTransactions(char ***matrix, int *activeTransCount, int startIndex, int endIndex);
int getIndexOfValue(char *value, char **matrix, int matrixSize);
char ***buildNewMatrixWithInitialFinalTrans(char ***matrix, int *newMatrixSize, uint startIndex, uint endIndex);
char ***readEntryFromStdin(int *linesQnt);

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

int *delimitSchedules(char ***matrix, int linesQnt, int *indexedQnt)
{
  char **activeTrans, *currentTrans;
  int *indexed, startIndexedSize, activeTransCount, indexOfExistantTrans, i, j;

  startIndexedSize = START_SIZE;
  indexed = calloc(startIndexedSize, sizeof(int));
  if (!indexed)
    return NULL;

  activeTransCount = 0;
  activeTrans = malloc(sizeof(char *));
  if (!activeTrans)
  {
    free(indexed);
    return NULL;
  }

  for (i = 0; i < linesQnt; i++)
  {
    currentTrans = calloc(sizeof(matrix[i][TRANSACTION_INDEX]), sizeof(char));
    if (!currentTrans)
    {
      j = activeTransCount - 1;
      while (j >= 0)
      {
        free(activeTrans[j]);
        j--;
      }

      free(activeTrans);
      free(indexed);

      return NULL;
    }
    memcpy(currentTrans, matrix[i][TRANSACTION_INDEX], sizeof(matrix[i][TRANSACTION_INDEX]));

    indexOfExistantTrans = -1;
    for (j = 0; j < activeTransCount && indexOfExistantTrans == -1; j++)
      if (!strcmp(activeTrans[j], currentTrans))
        indexOfExistantTrans = j;

    if (indexOfExistantTrans == -1 && strcmp(matrix[i][OPERATION_INDEX], COMMIT))
    {
      activeTrans[activeTransCount] = calloc(sizeof(currentTrans), sizeof(char));
      if (!activeTrans[activeTransCount])
      {
        j = activeTransCount - 1;
        while (j >= 0)
        {
          free(activeTrans[j]);
          j--;
        }

        free(activeTrans);
        free(indexed);

        return NULL;
      }

      memcpy(activeTrans[activeTransCount], currentTrans, sizeof(currentTrans));
      activeTransCount++;
    }
    else if (indexOfExistantTrans != -1 && !strcmp(matrix[i][OPERATION_INDEX], COMMIT))
    {
      free(activeTrans[indexOfExistantTrans]);
      memcpy(activeTrans + indexOfExistantTrans, activeTrans + indexOfExistantTrans + 1, (activeTransCount - indexOfExistantTrans - 1) * sizeof(char));
      activeTransCount--;
    }

    if (activeTransCount == 0)
    {
      if (*indexedQnt == startIndexedSize)
      {
        startIndexedSize *= 2;
        indexed = realloc(indexed, startIndexedSize * sizeof(int));
        if (!indexed)
        {
          j = activeTransCount - 1;
          while (j >= 0)
          {
            free(activeTrans[j]);
            j--;
          }

          free(activeTrans);
          free(indexed);

          return NULL;
        }
      }

      indexed[*indexedQnt] = i;
      (*indexedQnt)++;
    }

    free(currentTrans);
  }

  return indexed;
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

char **getActiveTransactions(char ***matrix, int *activeTransCount, int startIndex, int endIndex)
{
  int i, j, transExits;
  char **activeTrans;

  *activeTransCount = 0;
  activeTrans = malloc(sizeof(char *));
  if (!activeTrans)
    return NULL;

  for (i = startIndex; i <= endIndex; i++)
  {
    transExits = 0;
    for (j = 0; j < *activeTransCount && !transExits; j++)
      if (!strcmp(activeTrans[j], matrix[i][TRANSACTION_INDEX]))
        transExits = 1;

    if (!transExits)
    {
      activeTrans[*activeTransCount] = calloc(sizeof(matrix[i][TRANSACTION_INDEX]), sizeof(char));
      if (!activeTrans[*activeTransCount])
      {
        j = *activeTransCount - 1;
        while (j >= 0)
        {
          free(activeTrans[j]);
          j--;
        }
        free(activeTrans);

        return NULL;
      }

      memcpy(activeTrans[*activeTransCount], matrix[i][TRANSACTION_INDEX], sizeof(matrix[i][TRANSACTION_INDEX]));
      (*activeTransCount)++;
    }
  }

  return activeTrans;
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