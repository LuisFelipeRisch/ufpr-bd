#ifndef __CONFLICT_SERIALIZABLE_h__
#define __CONFLICT_SERIALIZABLE_h__

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <string.h>

uint **buildDependencyGraph(char ***matrix, uint startIndex, uint endIndex, uint *dependencyGraphSize);

#endif