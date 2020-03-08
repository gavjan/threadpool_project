#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"
#include "err.h"

struct QNode* newNode(runnable_t k) {
	struct QNode* tmp=malloc(sizeof(struct QNode));
	tmp->key=k;
	tmp->next=NULL;
	return tmp;
}

struct Queue* createQueue() {
	struct Queue* q=malloc(sizeof(struct Queue));
	if(q)
		q->front=q->rear=NULL;
	return q;
}

bool addQueue(struct Queue* q, runnable_t k) {
	struct QNode* tmp=newNode(k);
	if(!tmp) return false;
	if(q->rear == NULL) {
		q->front=q->rear=tmp;
		return true;
	}
	q->rear->next=tmp;
	q->rear=tmp;
	return true;
}

runnable_t popQueue(struct Queue* q) {
	int err=1;
	if(q->front == NULL) syserr(err, "queue empty");
	struct QNode* tmp=q->front;
	runnable_t key=tmp->key;
	q->front=q->front->next;
	free(tmp);
	if(q->front == NULL) q->rear=NULL;
	return key;
}

bool isEmptyQueue(struct Queue* q) {
	return q->front == NULL;
}

void destroyQueue(struct Queue* q) {
	while(!isEmptyQueue(q))
		popQueue(q);
	free(q);
}


