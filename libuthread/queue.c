#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include "queue.h"


//Queue Struct
struct queue {
	int length; //Length of Queue
	struct Node *head; //Head (for dequeuing)
	struct Node *tail; //Tail (for enqueueing)
};

//Each node in the queu has a val, and pointer to next
struct Node {
	void* val;
	struct Node *next;
};


//creates new queue struct, sets length to 0, and returns new Q
queue_t queue_create(void)
{
	struct queue *newQ = malloc(sizeof(struct queue *));
	newQ->length = 0;
	return newQ;

}


//frees memory associated with Queue
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
}

//enqgueues data by allocating new node and setting head to node
int queue_enqueue(queue_t queue, void *data)
{
	//check if data is null or if queue is null first
	if (data == NULL){
		return -1;
	}
	if (queue==NULL){
		return -1;
	}

	//make new node
	struct Node *newNode = malloc(sizeof(struct Node *));
	newNode->val = data;

	//if queue is empty, make head and tail new node
	if (queue->length == 0){
		queue->length++;
		queue->head = newNode;
		queue->tail = newNode;

	//if queue is not empty, add node to end of queue
	} else {
		queue->length++;
		queue->tail->next = newNode;
		queue->tail = newNode;
	}

	return 0;
}

//dequeue item (head of queue)
int queue_dequeue(queue_t queue, void **data)
{
	//check is queue or data is null
	if (!queue || !data){
		return -1;
	}
	if (queue->length==0){
		return -1;
	}
	
	//subtract length
	queue->length--;

	//pop queue
	*data= (queue->head->val);

	//set new head
	queue->head = queue->head->next;

	return 0;
}

//delete specific item from queue
int queue_delete(queue_t queue, void *data)
{
	//to avoid 'prevNode used uninitialized' error, make it static
	static struct Node *prevNode;

	//check if queue or data is null
	if (!queue || !data){
		return -1;
	}

	//iterate through queue
	struct Node *curNode = queue->head;
	while (curNode){

		//if we found a match
		if (curNode->val==data){

			
			queue->length--;

			//if match is head
			if (curNode==queue->head){
				if (queue->length >1){
					queue->head = queue->head->next;
				} else {
					queue_destroy(queue);
				}
			//if match is tail
			} else if (curNode==queue->tail){
				prevNode->next = NULL;
				
			//if match is in middle
			} else {

				prevNode->next = curNode->next;
				return 0;
			}
		}

		//continue with iteration
		prevNode = curNode;
		curNode = curNode->next;
	}

	return -1;
}

//iterate theough queue and perform funciton func
int queue_iterate(queue_t queue, queue_func_t func)
{
	//if queue or func is null
	if (!queue || !func){
		return -1;
	}

	//iterate through queue
	struct Node *curNode = queue->head;
	while (curNode){

		(*func)(queue, curNode->val);//perform func on node
		curNode = curNode->next;//continue iteration
	}

	return 0;
}



//return queue length
int queue_length(queue_t queue)
{
	//check to make sure queue isn't null
	if (!queue){
		return -1;
	}
	return queue->length;
}

