#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COL 4
#define TIME_INDEX 0
#define TRANSACTION_INDEX 1
#define OPERATION_INDEX 2
#define ATTRIBUTE_INDEX 3

#define READ "R"
#define WRITE "W"
#define COMMIT "C"

uint *allocUintArray(uint size);
int *allocIntArray(uint size);
int **allocIntMatrix(uint row, uint col);
uint **allocUintMatrix(uint row, uint col);
int *initIntArrayWith(int number, uint size);
uint *initUintArrayWith(uint number, uint size);
int **initIntMatrixWith(int number, uint row, uint col);
uint **initUintMatrixWith(uint number, uint row, uint col);

#endif