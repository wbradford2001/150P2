#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include "queue.h"

struct queue {
	/* TODO Phase 1 */
	int length;
	struct Node *head;
	struct Node *tail;
};

struct Node {
	void* val;
	struct Node *next;
};


queue_t queue_create(void)
{
	/* TODO Phase 1 */
	struct queue *newQ = malloc(sizeof(struct queue *));
	newQ->length = 0;
	return newQ;

}

int queue_destroy(queue_t queue)
{
	if (queue==NULL){
		return -1;
	} else if (queue->length!=0){
		return -1;
	}
	free(queue);
	queue=NULL;
	return 0;
	/* TODO Phase 1 */
}

int queue_enqueue(queue_t queue, void *data)
{

	
	if (data == NULL){
		return -1;
	}
	if (queue==NULL){
		return -1;
	}


	struct Node *newNode = malloc(sizeof(struct Node *));
	newNode->val = data;

	if (queue->length == 0){
		queue->length++;
		queue->head = newNode;
		queue->tail = newNode;
	} else {
		queue->length++;


		queue->tail->next = newNode;
		queue->tail = newNode;
	}

	return 0;
	/* TODO Phase 1 */
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if (!queue || !data){
		return -1;
	}
	if (queue->length==0){
		return -1;
	}
	
	queue->length--;



	//data = malloc(sizeof(typeof(queue->head->next)));
	// int c = 2;
	
	*data= (queue->head->val);
	queue->head = queue->head->next;



	return 0;
}


int queue_delete(queue_t queue, void *data)
{
	static struct Node *prevNode;


	if (!queue || !data){
		return -1;
	}
	struct Node *curNode = queue->head;
	while (curNode){
		//printf("%p, %p\n", curNode->val, data);
		if (curNode->val==data){
			queue->length--;
			if (curNode==queue->head){
				if (queue->length >1){
					queue->head = queue->head->next;
				} else {
					queue_destroy(queue);
				}
			} else if (curNode==queue->tail){
				prevNode->next = NULL;
				
			} else {

				prevNode->next = curNode->next;
				return 0;
			}
		}

		prevNode = curNode;
		curNode = curNode->next;
	}

	return -1;
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if (!queue || !func){
		return -1;
	}
	struct Node *curNode = queue->head;
	while (curNode){

		(*func)(queue, curNode->val);
		curNode = curNode->next;
	}

	return 0;
	/* TODO Phase 1 */
}


int queue_length(queue_t queue)
{
	if (!queue){
		return -1;
	}
	return queue->length;
	/* TODO Phase 1 */
}

