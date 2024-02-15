#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	/* TODO Phase 3 */
	int count;
	struct uthread_tcb *blockedTCB;


};

sem_t sem_create(size_t count)
{
	/* TODO Phase 3 */
	struct semaphore *newSem = malloc(sizeof(struct semaphore*));

	newSem->count = count;

	return newSem;

}

int sem_destroy(sem_t sem)
{
	free(sem);


	return 0;
	/* TODO Phase 3 */
}

int sem_down(sem_t sem)
{
	if (sem->count==0){
		// printf("count is 0\n");
		// if ((sem->blockedTCB==NULL)){
		sem->blockedTCB = &*(uthread_current)();
		//printf("%d\n",(sem->blockedTCB)->TID);
		uthread_block();
		
	} else {
		sem->count--;
	}


	return 0;
	/* TODO Phase 3 */
}

int sem_up(sem_t sem)
{

	if ((sem->blockedTCB)==NULL){

		sem->count++;
		return 0;
		
	}
	sem->count++;


	uthread_unblock(sem->blockedTCB);

	return 0;
	/* TODO Phase 3 */
}

