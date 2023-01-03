#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint *array;
  size_t used;
  size_t size;
} Array;

Array *initArray(uint initialSize);
int push(Array *array, uint element);
Array *removeElem(Array *array, uint indexToRemove);
void freeArray(Array *array);
int checkElemExistence(Array *array, uint element);
int isEmptyArray(Array *array);

#endif