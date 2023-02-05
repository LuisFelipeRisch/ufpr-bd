#ifndef __CONFLICT_SERIALIZABLE_h__
#define __CONFLICT_SERIALIZABLE_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "queue.h"
#include "khan.h"

int checkConflictSerializable(char ***matrix, Array *activeTrans, int startIndex, int endIndex);

#endif