# Introduction
Thank you for taking the time to read this report. We implemented the queue API
as a linked list with a pointer to the head node and tail node. queue_tester.x
contains unit tests for each function in the Queue API. We implemented 
the uthread library with a ready queue to be dequeued when the next 
process is to be run, with a blocked queue for processes that called sem_down(). 
Uthread produces the expected output for both uthread_yield and uthread_hello.
We implemented the preempt by creating an alarm signal every 10,000 miceoseconds
(10 times a second) that called a handler function that yielded the function 
automatically. We tested this in test_preempt.c by calling an infinite while
loop. When you run test_preempt.c with preempt=0 in uthread_run(), the
program halts and is stuck in the infinite while loop. However, if you
run with preempt = 1 in uthread_run(), the program continues. Details of our
implementation for each section are below
# Queue API
As mentioned in the introduction, we implemented the queue as a linked list
with a pointer to the head and a pointer to the tail. Each time the queue
was dequed, we return the head and set the head to the next node. Each time
an object was enqued, we set the tail.next to the new node and set the tail
to point to the newNode. This made deleting easy, as we only had to set the 
previous nodes next pointer to the next next node. Adjusting for edge cases
(like enqueing to an empty queue or deleting the last node) was the final touch.
# Queue Testing
We implemented rigorous testing in queue_tester.c. There are unit test for each 
function, and there is an enqueue-dequeue test as well. Each test comes with 
ASSERT statements to assert the output is correct.
# uthread
We created the TCB struct to help manage our threads, which contain pointers
to the TCB context, stack, and TID(Thread ID, which starts at 1 and goes
up for each subsequent thread created).
Our uthread library is implemented with a main queue and a blocked queue. 
The main queue contains all threads that have not been blocked, and 
oviously the blocked queue contains the queues that have called sem_down
with the sem at 0. The process of creating a thread is as follows: allocate
new memory for the thead, initialize TCB with the given functions in context.c,
and add thread to the main queue. The process of resuming/starting for the first
time is first dequeue the main q to get the next available TCB, switch the 
currentThread to the next TCB, and switch contexts. Right before a thread
yields and exists, it prepares the next thread to run and switches to the next
context (and in the case of yield, enques itself to run at a later time). 
We don't need a while loop because each time there is a thread changing event
(a thread is yielding, exiting, blocking or run) we dequeue the main Q and switch 
contexts. If a thread is exiting, we check if it is the last thread, and if so,
we call preempt_stop(because we call preempt_start in uthread_run())
Our thread library generates the expected output for both uthread_yield and 
uthread_hello. the uthread_block() function stops a thread and puts it in the 
blocked_queue. It also starts the next thread by dequeueing the main queue and
switching to its context. A blocked thread has to wait until another thread 
calls sem_up on its corrresponding semaphore for it to
be removed from the blocked queue and into the main queue. uthread_unblock()
does this by removing the TCB from the blocked queue and enqueuing it into the 
main Queue. 

# semaphore
To implement sem.c, we created a semaphore struct which has an int member called
count and a pointer to a queueu of threads is blocked by it. Sem_create
creates and returns an instance of a semaphore, and sem_desotry() frees 
the associate memory. sem_up() increments the count, while sem_down() 
decrements it. When a thread called sem_down() and the internal count is more 
than 0, theres nothing to worry about since the thread doesn't have to be 
blocked, so we just decrement count. If count is 0, however, we add the calling
thread to the blocked queue, and store the threads TCB in the sem's blockedTCBs
member. When we call sem_up(), if there are no threads being blocked by the 
sem(in other words, length of sem->blockedTCBs is 0) then we simply increament 
the count by 1. If, however, there are some blocked threads, we call 
u_thread_unblock.
# preempt
For preemption, we set up an alarm that would run once every 10,000 micro-
seconds (1,000,000 microseconds / second * 1 second / 100 interupts). The 
handler for this alarm simply called uthread_yield() and the thread yields
to the next thread. For disabling/enabling preemption, we simply disabled/
enabled the alarm signal.
# preempt testing
To test preemption, we set up a test case where one thread calls another, 
and there is an infinite while loop in the first thread. Without preemption
(if you replace uthread_run(1) with uthread_run(0)) the program prints out
"THREAD1" and then it reaches the infinite while loop it. This is expected 
because thread 1 never calls uthread_yield. With
premption, it prints out THREAD1 and then continues on to THREAD 2. This 
is because it automatically yields due to the alarm handler, so thread 2 can
run.
# Makefile
Our makefile contains the necessary flags (-Wall -Werror -Wextra) and 
a make clean rule. It also uses the principles discussed in discussion 
(no repetition, % to represent any files, $^/$@, dependancy tracking, 
debug output, ect...)
# sources
we used the professors given code and the help manuals given in the 
project description. 
# DISCLAIMER
One of our group members, William, has a brother, James, in this class,
and we all met up to talk about the problem from a conceptual standpoint,
and we each coded our own parts separately. 