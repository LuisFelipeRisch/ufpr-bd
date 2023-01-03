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
Array *removeElem(Array *array, uint element);
void freeArray(Array *array);

#endif