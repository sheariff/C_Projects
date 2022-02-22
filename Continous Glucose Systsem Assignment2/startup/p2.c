
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <process.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/siginfo.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sched.h>
#include "../.INCLUDE/qsm.h"          /* Required for using RTDisplay           */
#include "DataModule.h"
#include "DataArea.h"
#include "Sema.h"

#define MY_CYCLIC_PULSE _PULSE_CODE_MINAVAIL

#define RTsig1 SIGRTMIN
#define RTsig2 SIGRTMIN+1
#define RTsig3 SIGRTMIN+2
#define RTsig16 SIGRTMAX

void myHandler(int sig, siginfo_t *info, void *other) {

   switch (info->si_value.sival_int) {
   case 1: printf("voltage recieved");
   	   	     sensorSetValue(1, ptr_one-> voltage);
         ptr_one->Glucose =  ptr_one-> voltage;
             break;

   default:
             printf("Test1: Unexpected value of %d\n", info->si_value.sival_int);

   }

}

int main(int argc, char *argv[]) {
   /* Required for using RTDisplay           */
   /* Every program file accessing RTDisplay */
   /* must call ScreenInitialise().          */
	ScreenInitialise(1, 0);


	struct sigaction action;
	int retval;

	action.sa_handler = (void *)myHandler;
	sigfillset(&action.sa_mask);
	action.sa_flags = 0;
	if ((retval = sigaction(RTsig1,&action, NULL)) < 0) {
	    fprintf(stderr, "Main(): cannot add signal handler\n");
	    exit(retval);
	}

	/* Create 1 actuator      */

	sensorSetName(1,"Glucose sensor");

	while (1) {
	    sleep(1);
	}

}

