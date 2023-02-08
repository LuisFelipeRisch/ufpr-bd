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

int **allocIntMatrix(int row, int col);
int *initIntArrayWith(int number, int size);
int **initIntMatrixWith(int number, int row, int col);
Array *delimitSchedules(char ***matrix, int linesQnt);
Array *getActiveTransactions(char ***matrix, int startIndex, int endIndex);
char ***buildNewMatrixWithInitialFinalTrans(char ***matrix, int *newMatrixSize, int activeTransCount, int startIndex, int endIndex);
char ***readEntryFromStdin(int *linesQnt);

#endif