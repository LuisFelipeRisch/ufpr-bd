#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

#define COL 4

#define TIME_INDEX 0
#define TRANSACTION_INDEX 1
#define OPERATION_INDEX 2
#define ATTRIBUTE_INDEX 3

#define START_SIZE 5

#define READ "R"
#define WRITE "W"
#define COMMIT "C"

#define INITIAL_TRANS_IDENTIFIER "I"
#define FINAL_TRANS_IDENTIFIER "F"

uint *allocUintArray(uint size);
int *allocIntArray(uint size);
int **allocIntMatrix(uint row, uint col);
uint **allocUintMatrix(uint row, uint col);
int *initIntArrayWith(int number, uint size);
uint *initUintArrayWith(uint number, uint size);
int **initIntMatrixWith(int number, uint row, uint col);
uint **initUintMatrixWith(uint number, uint row, uint col);
int *delimitSchedules(char ***matrix, int linesQnt, int *indexedQnt);
void getTimestampsSchedule(char ***matrix, Array *timestamps, uint startIndex, uint endIndex);
int getUniqAttributesSchedule(char ***matrix, char **attributes, int *attributesCount, uint startIndex, uint endIndex);
char **getActiveTransactions(char ***matrix, int *activeTransCount, int startIndex, int endIndex);
int getIndexOfValue(char *value, char **matrix, int matrixSize);
char ***buildNewMatrixWithInitialFinalTrans(char ***matrix, int *newMatrixSize, uint startIndex, uint endIndex);
char ***readEntryFromStdin(int *linesQnt);

#endif