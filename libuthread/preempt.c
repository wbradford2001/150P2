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

//number if microseconds in a second
#define numMicroSecondsInSec 1000000


//sigsets
static sigset_t preemptAlarm;
static sigset_t oldSigSet;

//sigaction structs
static struct sigaction sigAct;
static struct sigaction oldSigAct;

//timer structs
static struct itimerval timer; 
static struct itimerval oldTimer;

//global variable to check if enabled
static bool enabled = false;



void preempt_disable(void)

{
	//check if disabled first
	if (!enabled){
		return;
	}

	//disable alarm
	sigprocmask(SIG_BLOCK,&preemptAlarm,NULL);
}

void preempt_enable(void)
{
	//check if disabled first
	if (!enabled){
		return;
	}

	//enable alarm
	sigprocmask(SIG_UNBLOCK,&preemptAlarm,NULL);

}


void handler(int sig) {
	//yield if you recieve alarm
    if (sig == SIGVTALRM) {
        uthread_yield();
    }
}


void preempt_start(bool preempt)
{
	//if no preempt, no point in doing anything, just return
	if (!preempt){
		return;
	} else {
		enabled = true;
	}

	//set up signal action
	sigAct.sa_handler=handler; 
	sigemptyset(&sigAct.sa_mask);
	sigAct.sa_flags=0;

	//set up preempt sig set
	sigemptyset(&preemptAlarm);
	sigaddset(&preemptAlarm,SIGVTALRM); 

	//initialize timer
	timer.it_interval.tv_usec = (float)((float)numMicroSecondsInSec)*(1/(float)(HZ));//convert 100  HZ to microsecond delay
	timer.it_value.tv_usec = (float)((float)numMicroSecondsInSec)*(1/(float)(HZ));//convert 100  HZ to microsecond delay
	//set new timer and store old timer
	setitimer(ITIMER_VIRTUAL,&timer,&oldTimer);

	//set new sigactions and store old sig act
	sigaction(SIGVTALRM,&sigAct,&oldSigAct);  

	//store old sigset
	sigprocmask(SIG_SETMASK,NULL,&oldSigSet);
}


void preempt_stop(void)
{
	//check if disabled
	if (!enabled){
		return;
	}
	//return old sigset, sigact, and timers
	sigprocmask(SIG_SETMASK,&oldSigSet,NULL);
	sigaction(SIGVTALRM,&oldSigAct,NULL);
	setitimer(SIGVTALRM,&oldTimer,NULL);
	
}

