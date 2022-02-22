//P3.c





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

   case 1: printf("pressure recieved and glucose level recieved");
   	   	     sensorSetValue(1, ptr_one-> pressure);
   	   	     sensorSetValue(2, ptr_one-> pressure);
             break;

   default:
             printf("Test1: Unexpected value of %d\n", info->si_value.sival_int);

   }

}

int main(int argc, char *argv[]) {
   /* Required for using RTDisplay           */
   /* Every program file accessing RTDisplay */
   /* must call ScreenInitialise().          */
	ScreenInitialise(2, 0);


	struct sigaction action;
	int retval;
	int chid;
	int rcvid;
    int     firstd;
    int 	secondd;
    int P6coid;
    union sigval sval;
    struct DataModule *ptr_one;
    struct DataArea *ptr_two;

	action.sa_handler = (void *)myHandler;
	sigfillset(&action.sa_mask);
	action.sa_flags = 0;
	if ((retval = sigaction(RTsig1,&action, NULL)) < 0) {
	    fprintf(stderr, "Main(): cannot add signal handler\n");
	    exit(retval);
	}
	firstd = shm_open("/DM1", O_RDWR, S_IRWXU);
    sleep(4);
    if (firstd == -1) {
           printf("P5: Error opening the data area: %s\n",
                        strerror(errno));
           exit(EXIT_FAILURE);

    }
    secondd = shm_open("/DM-1", O_RDWR, S_IRWXU);
    sleep(4);
    if (firstd == -1) {
           printf("P5: Error opening the data area: %s\n",
                        strerror(errno));
           exit(EXIT_FAILURE);

    }
	// Set up the pointer to the DataModule
    ptr_one = mmap(0, DATA_MODULE_SIZE,
                  PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ptr_two = mmap(0, DATA_AREA_SIZE,
                  PROT_READ|PROT_WRITE, MAP_SHARED, sd, 0);

   P6_coid = name_open(P3, 0);
	while (P6_coid == -1) {
		sleep(1);
		P6_coid = name_open(P6, 0);
	}

	sensorSetName(1, "pressure reading");
	sensorSetName(2, "glucose reading")


	while (1) {
         rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
	    sleep(1);
	    if (Ptr_one->pressure <= 120 || Ptr_one->pressure>= 80 && Ptr_one->glucose ==5 || Ptr_one ->glucose <= 10 )
            {
            printf("Blood pressure is normal");
            printf("blood sugar is normal")
            msg.type=REG_MSG
            strcpy(msg.message,"Bloodsugar is normal");
            if (MsgSend(P6_coid, &msg, sizeof(msg), NULL, 0)) {
								perror("P1: MsgSend");
								exit(EXIT_FAILURE);
						}

            }
        else if (Ptr_one->pressure > 120 && Ptr_one->glucose > 10 )
            {
            Printf("Signs of high blood pressure present");
            Printf("Blood sugar is high: sending alert");
            msg.type=REG_MSG
            strcpy(msg.message,"inject insulin");
            if (MsgSend(P6_coid, &msg, sizeof(msg), NULL, 0)) {
								perror("P1: MsgSend");
								exit(EXIT_FAILURE);
						}
            }
        else if (Ptr_one->pressure < 80&& Ptr_one->glucose < 5 )
            {
            Printf("Signs of low blood pressure present");
            Printf("Blood sugar is very low: sending alert");
            Printf("food consumption is required")
            msg.type=REG_MSG
            strcpy(msg.message,"food consumption required");
            if (MsgSend(P6_coid, &msg, sizeof(msg), NULL, 0)) {
								perror("P1: MsgSend");
								exit(EXIT_FAILURE);
						}

            }

	}

}


