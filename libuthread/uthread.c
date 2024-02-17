#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>



#include "private.h"
#include "uthread.h"
#include "queue.h"



static queue_t mainQ; //a.k.a. Ready Queue
static queue_t blockedQ; //threads's that are blocked because they called sem_down


static int numTCB=0;//stores the number of ThreadControlBlocks(TCB's)



//TCP struct
struct uthread_tcb {
	uthread_ctx_t *context; //pointer to context
	void *top_of_stack; //pointer to top of stack
	int TID; //Thread ID

};


static struct uthread_tcb *curThread; //current Thread
static struct uthread_tcb *mainTCB; //main Thread (created by thread run)


//returns pointer to current thread so SEM can block
struct uthread_tcb *uthread_current(void)
{
	return curThread;
}

//yields thread to next available thread
void uthread_yield(void)
{
	//set up new thread block to run after yield
	struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));


	queue_enqueue(mainQ, curThread);//enqueue current thread to run later

	queue_dequeue(mainQ, (void**)&nextTCB);//get next thread to run
	
	struct uthread_tcb *tmp = curThread;
	curThread = nextTCB;//update current thread
	//switch context
	uthread_ctx_switch(tmp->context, curThread->context);
}


//exits thread and executs next thread if necessary
void uthread_exit(void)
{


	//if there are no more threads left in queue
	if (queue_length(mainQ)==0){
		preempt_stop();
		return;
	}

	//set up next thread to run
	struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));
	queue_dequeue(mainQ, (void**)&nextTCB);
		
	//queue_enqueue(mainQ, curThread);
	//now we destroy old staack
	uthread_ctx_destroy_stack(curThread->top_of_stack);
	

	struct uthread_tcb *tmp = curThread;
	curThread = nextTCB;//set next current thread

	//switch contexts
	uthread_ctx_switch(tmp->context, curThread->context);
	
	
}

//creates new thread
int uthread_create(uthread_func_t func, void *arg)
{

	preempt_disable();

	//set up and initialize new thread
	struct uthread_tcb *newTCB = malloc(sizeof(struct uthread_tcb*));
	newTCB->top_of_stack= uthread_ctx_alloc_stack();
	newTCB->context = malloc(sizeof(uthread_ctx_t));
	numTCB++;
	newTCB->TID=numTCB;
    if (uthread_ctx_init(newTCB->context, newTCB->top_of_stack, (uthread_func_t)func, arg) == -1) {
        numTCB--;
        uthread_ctx_destroy_stack(newTCB->top_of_stack);
        free(newTCB);
        return -1; 
    }	
	//uthread_ctx_init(newTCB->context, newTCB->top_of_stack,func, arg);

	preempt_enable();

	//put new thread in queue
	queue_enqueue(mainQ, newTCB);
	return 0;
}

//kick off uthread library
int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	(void)preempt;
	preempt_start(0);

	preempt_disable();

	//set up and initializze main thread
    mainTCB = malloc(sizeof(struct uthread_tcb*));
	mainTCB->top_of_stack= uthread_ctx_alloc_stack();
	mainTCB->context = malloc(sizeof(uthread_ctx_t));

	//create main Q and blockedQ
	blockedQ=queue_create();
	mainQ=queue_create();


	//set up and initialize new thread
	struct uthread_tcb *newTCB = malloc(sizeof(struct uthread_tcb*));
	newTCB->top_of_stack= uthread_ctx_alloc_stack();
	newTCB->context = malloc(sizeof(uthread_ctx_t));
	numTCB++;
	newTCB->TID=numTCB;
    if (uthread_ctx_init(newTCB->context, newTCB->top_of_stack, (uthread_func_t)func, arg) == -1) {
        numTCB--;
        uthread_ctx_destroy_stack(newTCB->top_of_stack);
        free(newTCB);
        return -1; 
    }	


	//put new thread in queue
	queue_enqueue(mainQ, newTCB);



	//enqueue newTCB

	preempt_enable();

	
		//get new thread
		queue_dequeue(mainQ, (void**)&curThread);
		//switch contexts
		uthread_ctx_switch(mainTCB->context, curThread->context);
	
	return 0;

}

//block uthread (because it called sem)
void uthread_block(void)
{


	//put current thread in blocked queue
	queue_enqueue(blockedQ, curThread);

	struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));
	queue_dequeue(mainQ, (void**)&nextTCB);
	
	//set up and allocate next thread ot run
	struct uthread_tcb *tmp = curThread;
	curThread = nextTCB;//update current thread



	
	uthread_ctx_switch(tmp->context, nextTCB->context);//switch contexts

}

//unblock thread because its semaphore has been freed
void uthread_unblock(struct uthread_tcb *uthread)
{


    queue_enqueue(mainQ, (void**)uthread);

    queue_delete(blockedQ, &uthread);
	


}

