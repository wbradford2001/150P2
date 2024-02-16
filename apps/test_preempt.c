/*
This is similar  the uthread_yield test case but with an infinite loop. 
If you enter 0 as the preempt argument for uthread_run(preempt), the program is stuck in an infinite loop
 */



#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>


//Thread 2 wont run unless preempt is enabled
void thread2(void *arg)
{
	(void)arg;
	uthread_yield();
	printf("THREAD2\n");
}


void thread1(void *arg)
{
	(void)arg;

	uthread_create(thread2, NULL);
	uthread_yield();
	printf("THREAD1\n");


	//INFINITE LOOP
	while (1){
		//If preempt is not enabled, this loop will run indefenitely
	}


	uthread_yield();
}

int main(void)
{
	//start off thread process
	uthread_run(1, thread1, NULL);
	return 0;
}
