#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  int *array;
  size_t used;
  size_t size;
} Array;

Array *initArray(int initialSize);
int push(Array *array, int element);
Array *removeElem(Array *array, int indexToRemove);
void freeArray(Array *array);
int checkElemExistence(Array *array, int element);
int isEmptyArray(Array *array);

#endif