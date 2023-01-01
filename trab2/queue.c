#include "queue.h"

Queue *initQueue(){
  Queue *queue = malloc(sizeof(Queue));

  if(!queue)
    return NULL;

  queue->root = NULL;
  queue->end = NULL;

  return queue;
}

int enqueue(Queue *queue, uint data){
  Node *node = malloc(sizeof(Node));
  if(!node)
    return 0;

  Node *root = queue->root;
  node->data = data;
  node->leftNode = NULL;
  node->nextNode = NULL;

  if(!root){
    queue->root = node;
    queue->end = node;
  } else {
    node->nextNode = root;
    root->leftNode = node;
    queue->root = node;
  }

  return 1;
}

uint dequeue(Queue *queue){
  uint data;
  Node *end = queue->end;
  Node *root = queue->root;
  if(!end && !root)
    return 0;

  data = end->data;

  if(root == end){
    queue->root = NULL;
    queue->end = NULL;
  } else{
    queue->end = end->leftNode;
  }

  free(end);
  return data;
}

int isEmpty(Queue *queue){
  return queue->root == NULL;
}

void cleanQueue(Queue *queue){
  Node *node = queue->root;

  while (node)
  {
    Node *tempNode = node;
    node = node->nextNode;
    free(tempNode);
  }
  free(queue);
}