


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

int main() {
	   /* Required for using RTDisplay           */
	   /* Every program file accessing RTDisplay */
	   /* must call ScreenInitialise().          */
		ScreenInitialise(2, 0);
		sleep(2);
		struct sigevent event;
		struct itimerspec itime;
		timer_t timer_id;
		int chid;
		int rcvid;
	    int     fd;
	    int 	sd;
	    int P4_coid;
	    union sigval sval;
	    struct DataModule *ptr_one;
	    char insulin [50];

		int temperature;
		sem_t *sem = sem_open(SNAME, 0);


		chid = ChannelCreate(0);
		event.sigev_notify = SIGEV_PULSE;
		event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid,
												 _NTO_SIDE_CHANNEL, 0);
		event.sigev_priority = -1;
		timer_create(CLOCK_REALTIME, &event, &timer_id);

		// Open the DataModule.
	    fd = shm_open("/Datamodule", O_RDWR, S_IRWXU);
	    sleep(4);
	    if (fd == -1) {
	           printf("P5: Error opening the data area: %s\n",
	                        strerror(errno));
	           exit(EXIT_FAILURE);

	    }

		// Set up the pointer to the DataModule
	    ptr_one = mmap(0, DATA_MODULE_SIZE,
	                  PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	    firstd = shm_open("/DM1", O_RDWR, S_IRWXU);
        sleep(4);
        if (firstd == -1) {
           printf("P5: Error opening the data area: %s\n",
                        strerror(errno));
           exit(EXIT_FAILURE);

        }

		// Attach communication channel to P4 once it becomes available
		int P4_coid = name_open(P4, 0);
		while (P4_coid == -1) {
			sleep(1);
			P4_coid = name_open(P4, 0);
		}

		while(1) {
		   rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);
		 if (Insulin == "empty")
            {
             msg.type=REG_MSG
            strcpy(msg.message,"insulin cartridge is empty");
            if (MsgSend(P6_coid, &msg, sizeof(msg), NULL, 0)) {
								perror("P1: MsgSend");
								exit(EXIT_FAILURE);
						}
            }
            else{
            msg.type=REG_MSG
            strcpy(msg.message,"insulin cartridge is full");
            if (MsgSend(P6_coid, &msg, sizeof(msg), NULL, 0)) {
								perror("P1: MsgSend");
								exit(EXIT_FAILURE);
		   }}

		}
}


