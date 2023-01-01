#ifndef __KHAN_H__
#define __KHAN_H__

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "queue.h"

int khan(uint **dependencyGraph, uint dependencyGraphSize, Queue *serializedTransactions);

#endif