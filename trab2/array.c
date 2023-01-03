#include "array.h"

Array *initArray(uint initialSize){
  Array *array = malloc(sizeof(Array));

  if(!array)
    return NULL;

  array->size = initialSize;
  array->used = 0;

  array->array = calloc(initialSize, sizeof(uint));
  if(!array->array){
    free(array);
    return NULL;
  }

  return array;
}

int push(Array *array, uint element){
  if(array->used == array->size){
    array->size *= 2;
    array->array = realloc(array->array, array->size * sizeof(int));

    if(!array->array)
      return -1;
  }

  array->array[array->used++] = element;
  return element;
}

void freeArray(Array *array) {
  free(array->array);
  free(array);
}

Array *removeElem(Array *array, uint element){
  Array *temp = initArray(array->size);
  if(!temp)
    return array;

  int indexToRemove = -1;
  for(uint i = 0; i < array->used; i++)
    if(array->array[i] == element)
      indexToRemove = i;

  if(indexToRemove == -1)
  {
    freeArray(temp);
    return array;
  }

  memcpy(temp->array, array->array, indexToRemove * sizeof(uint));
  memcpy(temp->array + indexToRemove, array->array + indexToRemove + 1, (array->size - indexToRemove - 1) * sizeof(uint));
  temp->used = array->used - 1;
  freeArray(array);

  return temp;
}