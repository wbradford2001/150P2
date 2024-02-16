#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100


static sigset_t preemptAlarm;
static sigset_t oldSigSet;


static struct sigaction sigAct;
static struct sigaction oldSigAct;


static struct itimerval timer; // when a timer should expire
static struct itimerval oldTimer;

static bool enabled;



void preempt_disable(void)

{
	if (!enabled){
		return;
	}
	/* TODO Phase 4 */
	sigprocmask(SIG_BLOCK, &preemptAlarm, NULL);
}

void preempt_enable(void)
{
	if (!enabled){
		return;
	}
	/* TODO Phase 4 */
	sigprocmask(SIG_UNBLOCK, &preemptAlarm, NULL);

}


void handler(int sig) {

    if (sig == SIGVTALRM) {
		//printf("kjdnfg\n");
        uthread_yield();
    }
}


void preempt_start(bool preempt)
{
	if (!preempt){
		return;
	} else {
		enabled = true;
	}
	sigAct.sa_handler = handler; 
	sigemptyset(&sigAct.sa_mask);
	sigAct.sa_flags = 0;
	timer.it_interval.tv_sec = 0; 
	timer.it_value.tv_sec = 0;
	timer.it_interval.tv_usec = (long int) (1.0 / HZ * 1000000);
	timer.it_value.tv_usec = (long int) (1.0 / HZ * 1000000);
	sigemptyset(&preemptAlarm);
	sigaddset(&preemptAlarm, SIGVTALRM); 
	sigaction(SIGVTALRM, &sigAct, &oldSigAct);  
	sigprocmask(SIG_SETMASK, NULL, &oldSigSet); // keep track of old set
	setitimer(ITIMER_VIRTUAL, &timer, &oldTimer);
}

void preempt_stop(void)
{
	if (!enabled){
		return;
	}
	sigprocmask(SIG_SETMASK, &oldSigSet, NULL);


	sigaction(SIGVTALRM, &oldSigAct, NULL);


	setitimer(SIGVTALRM, &oldTimer, NULL);
	
}

