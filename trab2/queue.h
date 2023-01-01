#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
  uint data;
  struct Node *nextNode;
  struct Node *leftNode;
} Node;

typedef struct
{
  struct Node *root;
  struct Node *end;
} Queue;

Queue *initQueue();
int enqueue(Queue *queue, uint data);
uint dequeue(Queue *queue);
int isEmpty(Queue *queue);
void cleanQueue(Queue *queue);

#endif