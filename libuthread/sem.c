#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"


//semaphore struct
struct semaphore {
	queue_t blockedTCBs;
	int count;
	
};

//creates a semaphore
sem_t sem_create(size_t count)
{
	struct semaphore *newSem = malloc(sizeof(struct semaphore*));
	
	newSem->blockedTCBs = queue_create();
	newSem->count = count;
	return newSem;
}

//destroys a semaphore(frees memory)
int sem_destroy(sem_t sem)
{
	free(sem);
	return 0;
}

//takes down value of semaphore
int sem_down(sem_t sem)
{
	//if sem is blocked
	if (sem->count==0){

		queue_enqueue(sem->blockedTCBs,uthread_current());//set current thread as blocked
		

		uthread_block();//call block on the thread

		
	} else {
		//if sem is positive, nothing to worry about, decrement sem
		sem->count--;
	}

	return 0;

}


//increase value of sem
int sem_up(sem_t sem)
{
	//if there is no thread being blocked by semaphore
	if (queue_length(sem->blockedTCBs) == 0){
		//increament count
		sem->count++;
		return 0;
	}

	//if thre is a thread being blocked
	struct uthread_tcb *nextThread = malloc(sizeof(struct uthread_tcb*));
	queue_dequeue(sem->blockedTCBs, (void**)&nextThread);
	uthread_unblock(nextThread);

	return 0;
}

