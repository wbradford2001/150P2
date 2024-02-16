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


//internal function for debugging to print queue
// static void print_queue(queue_t q, void *data)
// {
// 	(void)q;
//     printf("TID: %d\n", ((struct uthread_tcb*)data)->TID);
// }


void uthread_yield(void)
{
	/* TODO Phase 2 */
//	printf("yielding now: %d\n", curThread->TID);




	struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));
	queue_enqueue(mainQ, curThread);
	queue_dequeue(mainQ, (void**)&nextTCB);
	
	struct uthread_tcb *tmp = curThread;
	curThread = nextTCB;
	uthread_ctx_switch(tmp->context, nextTCB->context);
}

void uthread_exit(void)
{

	struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));


	
	if (queue_length(mainQ)==0){
		return;
	}
	//queue_iterate(mainQ,print_queue);
	queue_dequeue(mainQ, (void**)&nextTCB);
		
	queue_enqueue(mainQ, curThread);
	
	struct uthread_tcb *tmp = curThread;
	curThread = nextTCB;
	if (queue_length(mainQ)==1){
		preempt_stop();
	}

	uthread_ctx_switch(tmp->context, nextTCB->context);
	
	
	/* TODO Phase 2 */
}

int uthread_create(uthread_func_t func, void *arg)
{
	/* TODO Phase 2 */
	//printf("creating thread: %d\n", numTCB+1);

	preempt_disable();
	struct uthread_tcb *newTCB = malloc(sizeof(struct uthread_tcb*));
	
	newTCB->top_of_stack= uthread_ctx_alloc_stack();

	newTCB->context = malloc(sizeof(uthread_ctx_t));
	numTCB++;
	newTCB->TID=numTCB;

	preempt_enable();


	uthread_ctx_init(newTCB->context, newTCB->top_of_stack,func, arg);

	queue_enqueue(mainQ, newTCB);
	return 0;
	//queue_iterate(mainQ, print_queue);
	//printf("B: %d\n", queue_length(mainQ));
}


int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	//printf("running thread\n");


	preempt_start(preempt);

	preempt_disable();

	mainTCB = malloc(sizeof(struct uthread_tcb*));
	
	mainTCB->top_of_stack= uthread_ctx_alloc_stack();

	mainTCB->context = malloc(sizeof(uthread_ctx_t));

	// uthread_ctx_init(mainTCB->context, mainTCB->top_of_stack,loop, arg);





	struct uthread_tcb *newTCB = malloc(sizeof(struct uthread_tcb*));
	
	newTCB->top_of_stack= uthread_ctx_alloc_stack();

	newTCB->context = malloc(sizeof(uthread_ctx_t));
	numTCB++;
	newTCB-> TID = numTCB;

	uthread_ctx_init(newTCB->context, newTCB->top_of_stack,func, arg);


	mainQ=queue_create();
	blockedQ=queue_create();



	queue_enqueue(mainQ, newTCB);

	preempt_enable();

	while (1){

		queue_dequeue(mainQ, (void**)&curThread);
		uthread_ctx_switch(mainTCB->context, curThread->context);
	}




	
}

void uthread_block(void)
{
//	printf("Blocking thread: %d\n", curThread->TID);
	/* TODO Phase 3 */
	queue_enqueue(blockedQ, curThread);

	//queue_enqueue(mainQ, curThread);

	struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));
	//queue_enqueue(mainQ, curThread);
	queue_dequeue(mainQ, (void**)&nextTCB);
	
	struct uthread_tcb *tmp = curThread;
	curThread = nextTCB;
	uthread_ctx_switch(tmp->context, nextTCB->context);

}

// static int TID_to_search_for;
// static void iterator_inc(queue_t q, void *data)
// {
//     int a = ((struct uthread_tcb*)data)->TID;

//     if (a == TID_to_search_for)
//         queue_delete(q, data);

// }

void uthread_unblock(struct uthread_tcb *uthread)
{
	//printf("UnBlocking thread: %d\n", uthread->TID);

	// TID_to_search_for = uthread->TID;
	// //struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));
	// queue_iterate(blockedQ, iterator_inc);

	queue_enqueue(mainQ, uthread);




	struct uthread_tcb *nextTCB = malloc(sizeof(struct uthread_tcb*));
	//queue_enqueue(mainQ, curThread);
	queue_dequeue(mainQ, (void**)&nextTCB);
	
	struct uthread_tcb *tmp = curThread;
	curThread = nextTCB;
	uthread_ctx_switch(tmp->context, nextTCB->context);


}

