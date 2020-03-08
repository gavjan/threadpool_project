#ifndef QUEUE_H
#define QUEUE_H

#include "runnable.h"

struct QNode {
	runnable_t key;
	struct QNode* next;
};

typedef struct Queue {
	struct QNode* front;
	struct QNode* rear;
} Queue;

struct Queue* createQueue();

bool addQueue(struct Queue* q, runnable_t k);

runnable_t popQueue(struct Queue* q);

bool isEmptyQueue(struct Queue* q);

void destroyQueue(struct Queue* q);
#endif //QUEUE_H
