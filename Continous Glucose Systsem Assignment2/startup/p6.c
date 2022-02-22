

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

int main(int argc, char *argv[]) {

	name_attach_t *att;
	int rcvid;
	struct _msg_info msg_info;
	int msg;
	screenInitilise(0,1);
	ActuatorSetName(1, Pump Motor);
    char message [50];

	// Attach name P2 for connection ID.
	att = name_attach(NULL, P6, 0);
    sleep(4);


	if (att == NULL) {
		perror("P2: name_attach()");
		exit(EXIT_FAILURE);
	}
	//  Main receiving loop.
	while (1) {
		// Wait until a message is received at the channel.
		rcvid = MsgReceive(att->chid, &msg, sizeof(msg), &msg_info);

		// See if a message failure, start again.
		if (rcvid == -1) {
			perror("P2: MsgReceive failed");
			continue;

		}
	// *See if pulse, ignore and start again.
		if (rcvid == 0) {
			continue;
		}

		switch (msg.type)
		{
        case REG_MSG:
            MsgReply(rcvid, 0 , NULL,0)
            printf(PROGNAME, strcpy(message,"message recieved )");
            printf(PROGNAME, " = '%s' \n", msg.message);
            if (message == "Bloodsugar is normal" ||message == "food consumption required"){
                  ActuatorSetValue(1, "normal: do nothing")
                }
               else if(message == "inject insulin"){
                        ActuatorSetValue(1, "pump insulin")
                   }

            break;
        default:
            printf(PROGNAME, "unexpected message type: %d\n", msg.type);
            msg.Error(rcvid,ENOSYS);
            break;
		}



	}

}






