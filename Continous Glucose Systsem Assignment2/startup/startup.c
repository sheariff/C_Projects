/*
 * Q5 -> startup.c
 *
 *  Created on: 27 Feb 2020
 *      Author: Avinash Nagarajan
 *      Description: Root process that spawns five processes and creates two data modules.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <spawn.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <process.h>
#include <sys/neutrino.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "DataArea.h"
#include "DataModule.h"
#include "Sema.h"
#include "../.INCLUDE/qsm.h"          /* Required for using RTDisplay           */

int main (int argc, char * argv[], char **envp) {
	               /* Create 3 sensors and 4 actuators.      */
    int firstd;
    int secondd;
    struct DataArea *ptr_one;
    struct DataModule *ptr_two;

    /* Create the data area.                */
    firstd = shm_open("/DM-1", O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    if (firstd == -1) {
           printf("DM1: Error creating the shared memory object. %s.\n",strerror(errno));
           exit(EXIT_FAILURE);
    }

    /* Set the size data area.              */
    secondd = shm_open("/DM-2", O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    if (secondd == -1) {
           printf("DM2: Error creating the shared memory object. %s.\n",strerror(errno));
           exit(EXIT_FAILURE);
    }

    ftruncate(firstd, DATA_AREA_SIZE);
    ftruncate(secondd, DATA_MODULE_SIZE);

    /* Make 'ptr'-one, -two point into the data area. */
    ptr_one = mmap(0, DATA_AREA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ptr_two = mmap(0, DATA_MODULE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, sd, 0);

    // Create the Seamophore
    sem_t *sem = sem_open(SNAME, O_CREAT, 0644, 1);

    // Creating the necessary data types for spawning the processes
	union sigval sval;
	char    *args1[] = {"P1", "-l", NULL};
	int     status1;
	pid_t   pid1;
	struct inheritance inherit1;

	char    *args2[] = {"P2", "-l", NULL};
	int     status2;
	pid_t   pid2;
	struct inheritance inherit2;

	char    *args3[] = {"P3", "-l", NULL};
	int     status3;
	pid_t   pid3;
	struct inheritance inherit3;

	char    *args4[] = {"P4", "-l", NULL};
	int     status4;
	pid_t   pid4;
	struct inheritance inherit4;

	char    *args5[] = {"P5", "-l", NULL};
	int     status5;
	pid_t   pid5;
	struct inheritance inherit5;

	char    *args6[] = {"P6", "-l", NULL};
	int     status6;
	pid_t   pid6;
	struct inheritance inherit6;

	// Spawn P1
	inherit1.flags = 0;
	if ((pid1 = spawn(args1[0], 0, NULL, &inherit1, args1, envp)) == -1)
		perror("P1: spawn() failed");
	else
		printf("P1: spawned child, pid ==> %d\n", pid1);

	// Spawn P2
	inherit2.flags = 0;
	if ((pid2 = spawn(args2[0], 0, NULL, &inherit2, args2, envp)) == -1)
	   perror("P2: spawn() failed");
	else
	   printf("P2: spawned child, pid ==> %d\n", pid2);

	// Spawn P3
	inherit3.flags = 0;
	if ((pid3 = spawn(args3[0], 0, NULL, &inherit3, args3, envp)) == -1)
	   perror("P3: spawn() failed");
	else
	   printf("P3: spawned child, pid ==> %d\n", pid3);

	// Spawn P4
	inherit4.flags = 0;
	if ((pid4 = spawn(args4[0], 0, NULL, &inherit4, args4, envp)) == -1)
	   perror("P4: spawn() failed");
	else
	   printf("P4: spawned child, pid ==> %d\n", pid4);

	// Spawn P5
	inherit5.flags = 0;
	if ((pid5 = spawn(args5[0], 0, NULL, &inherit5, args5, envp)) == -1)
	   perror("spawn() failed");
	else
	   printf("P5: spawned child, pid ==> %d\n", pid5);
	   inherit5.flags = 0;

	if ((pid6 = spawn(args6[0], 0, NULL, &inherit6, args6, envp)) == -1)
	   perror("spawn() failed");
	else
	   printf("P6: spawned child, pid ==> %d\n", pid6);


	ptr_two -> pid[1] = pid1;
	ptr_two -> pid[2] = pid2;
	ptr_two -> pid[3] = pid3;
	ptr_two -> pid[4] = pid4;
	ptr_two -> pid[5] = pid5;
	ptr_two -> pid[6] = pid6;

	/* Wait for the child P1 to finish.*/
	if (waitpid(pid1, &status1, WEXITED) <= 0)
		perror("process1: Error with waiting???");
	else
		printf("process1: Process pid=%d closed successfully and status==>%d\n",pid1, status1);

	/* Wait for the child P2 to finish.*/
	if (waitpid(pid2, &status2, WEXITED) <= 0)
		perror("process2: Error with waiting???");
	else
		printf("process2: Process pid=%d closed successfully and status==>%d\n",pid2, status2);

	/* Wait for the child P3 to finish.*/
	if (waitpid(pid3, &status3, WEXITED) <= 0)
		perror("process1: Error with waiting???");
	else
		printf("process1: Process pid=%d closed successfully and status==>%d\n",pid3, status3);

	/* Wait for the child P4 to finish.*/
	if (waitpid(pid4, &status4, WEXITED) <= 0)
		perror("process1: Error with waiting???");
	else
		printf("process1: Process pid=%d closed successfully and status==>%d\n",pid4, status4);

	/* Wait for the child P5 to finish.*/
	if (waitpid(pid5, &status5, WEXITED) <= 0)
		perror("process1: Error with waiting???");
	else
		printf("process1: Process pid=%d closed successfully and status==>%d\n",pid5, status5);

    if (waitpid(pid6, &status6, WEXITED) <= 0)
		perror("process1: Error with waiting???");
	else
		printf("process1: Process pid=%d closed successfully and status==>%d\n",pid6, status6);


	sleep(60);
    printf("60 seconds passed --- Data Area will be closed/deleted\n");
    /* Closing the file descriptor */
    close(firstd);
    close(secondd);

    /* removing the mapping.       */
    munmap(ptr_one, DATA_AREA_SIZE);
    munmap(ptr_two, DATA_MODULE_SIZE);

    /* Delete the data area.       */
    shm_unlink("/DM-1");
    shm_unlink("/DM-2");

	return(0);
 }
