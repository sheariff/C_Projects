

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

typedef union {

struct _pulse pulse;

     /* other message structures would go here too */

} my_message_t;

int main(int argc, char *argv[]) {
   /* Required for using RTDisplay           */
   /* Every program file accessing RTDisplay */
   /* must call ScreenInitialise().          */
	ScreenInitialise(2, 0);
	struct sigevent event;
	struct itimerspec itime;
	timer_t timer_id;
	int chid;
	int rcvid;
    int  firstd0
    int  secondd;
    int P2_coid;
    union sigval sval;
    struct DataModule *ptr_one;
    struct DataArea *ptr_two;
	int pressure;
	int electrochemical
	sem_t *sem = sem_open(SNAME, 0);

	SensorSetName(1, "pressureSensor");
	SensorSetName(2, "electrochemical-Sensor");
	my_message_t msg;
	chid = ChannelCreate(0);
	event.sigev_notify = SIGEV_PULSE;
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, chid,
											 _NTO_SIDE_CHANNEL, 0);
	event.sigev_priority = -1;
	event.sigev_code = MY_CYCLIC_PULSE;
	timer_create(CLOCK_REALTIME, &event, &timer_id);

	// Open the DataModule.
    firstd = shm_open("/DataModule2", O_RDWR, S_IRWXU);
    sleep(4);
    if (firstd == -1) {
           printf("P1: Error opening the data area: %s\n",
                        strerror(errno));
           exit(EXIT_FAILURE);

    }
    sd = shm_open("/DataModule1", O_RDWR, S_IRWXU);
    sleep(4);
    if (secondd == -1) {
           printf("P1: Error opening the data area: %s\n",
                        strerror(errno));
           exit(EXIT_FAILURE);

    }
	// Set up the pointer to the DataModule
    ptr_one = mmap(0, DATA_MODULE_SIZE,
                  PROT_READ|PROT_WRITE, MAP_SHARED, firstd, 0);
    ptr_two = mmap(0, DATA_AREA_SIZE,
                  PROT_READ|PROT_WRITE, MAP_SHARED, secondd, 0);


    // Period for the cyclic process
	itime.it_value.tv_sec = 500;
	itime.it_value.tv_nsec = 0;
	itime.it_interval.tv_sec = 500;
	itime.it_interval.tv_nsec = 0;

	//  communication channel to P2
	P2_coid = name_open(P2, 0);
	while (P2_coid == -1) {
		sleep(1);
		P2_coid = name_open(P2, 0);
	}
//  communication channel to P3
	P3_coid = name_open(P3, 0);
	while (P3_coid == -1) {
		sleep(1);
		P2_coid = name_open(P3, 0);
	}

	timer_settime(timer_id, 0, &itime, NULL);
	while (1) {
       // Wait for a PULSE every period.
	   rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);

	   if(rcvid == 0) {

		   // Check to see if our CYCLIC pulse.
		   if (msg.pulse.code == MY_CYCLIC_PULSE) {
			   pressure = SensorGetValue(1);
			   electrochemical = SensorGetValue(1);
			   printf("  Reading:%d\n", pressure);
			   printf("  Reading:%d\n", electrochemical);
			       sem_wait(sem);
			   	   ptr_one -> pressure = pressure;
			   	   ptr_one -> Voltage = Electrochemical;
                   strcpy(ptr_one -> message, "pressure is being sent");
						// Send message msg to P2 connection.
						if (MsgSend(P2_coid, &(ptr_one->pressure), sizeof(ptr_one->pressure), NULL, 0)) {
								perror("P1: MsgSend");
								exit(EXIT_FAILURE);
						}

			   	   }
			   	   sleep(10);
			   	   sem_post(sem);
				   sval.sival_int = electrochemical;
				   sigqueue(ptr_two->pid[2], RTsig1, sval);
				   sval.sival_int = pressure;
				   sigqueue(ptr_two->pid[3], RTsig1, sval);
				   sleep(10);
			   }
		   }
		}
	}
}





