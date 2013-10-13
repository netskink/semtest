/*++
 *
 * Semtest program.
 *
 * --*/


#include "dbgzero.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "common.h"
#include "usrlib.h"



#pragma warning(disable : 4995)
#pragma warning(disable : 4242)
#pragma warning(disable : 4244)
#pragma warning(disable : 4090)
#pragma warning(disable : 4047)
#pragma warning(disable : 4313)

#include <sys/cdefs.h>
#include <sys/param.h>
//#include <sys/sem.h>
#include <linux/sem.h>
//#include <sys/shm.h>
//#include <sys/msg.h>
//#include <sys/ipc.h>

#include <ctype.h>
#include <err.h>
#include <grp.h>
#include <unistd.h>


#include "semgen.h"
#ifdef movedtolinusr

struct sembuf sem_unlock = {0, /* member # in set (0,1, ..., nsems - 1) */
							1, /* operation (negative, 0, or positibe) */
							IPC_NOWAIT};	/* IPC_NOWAIT or SEM_UNDO */



struct sembuf sem_lock   = {0, /* member # in set (0,1, ..., nsems - 1) */
							-1, /* operation (negative, 0, or positibe) */
							0};	/* IPC_NOWAIT or SEM_UNDO */


struct sembuf sem_lock_nowait   = {0, /* member # in set (0,1, ..., nsems - 1) */
							-1, /* operation (negative, 0, or positibe) */
							IPC_NOWAIT};	/* IPC_NOWAIT or SEM_UNDO */

//struct sembuf my_sem_operations[10];

/* The set of semaphores.
 * 1  -- shared resource is available.
 * 0  -- shared resource is being used.
 */
//short  sarray[NUMSEMS];

/* errno basic defines found here
 * /usr/include/asm/errno.h
 **/



#if 0
create_sem_key () {
	semkey = ftok(SEMKEYPATH,SEMKEYID);
	if ( semkey == (key_t)  -1 ) {
		myprint("ftok() for sem failure.\n");
		my_exit(-1);
	}
}
#endif


/*
 * rc = -1 if failed or if it fails
 * rc = 0 if it locks the semaphore
 */
int mysemtimedwait(int semid) {

	int rc, status, loopcounter = 0;

	while (loopcounter < 5 ) {
		/*
		 * Lock the semaphore
		 */
		rc = semop(semid, &sem_lock_nowait, NUMOPS);
		myprint("rc = %d \n", rc);
		if ( rc == 1 ) {
			myprint("lock failed\n");
			myprint("fatal error exit \n");
			return(-1);
		} else if (rc == EAGAIN )  {
			myprint("sleeping\n");
			sleep(2);
			loopcounter++;
		} else if (rc == 0 )  {
			myprint("locked no wait worked.  We have the resource.\n");
			loopcounter = 5;
			rc = 0;
		} else {
			status = my_get_last_error();
			myprint("semget else cond.  my_get_last_error is %ld\n",status);
			if (EAGAIN == status) {
				myprint("sleeping\n");
				sleep(2);
				loopcounter++;
			}
		}
	}

	readline("hit return to continue. We have ended the mytimedwait routine.\n");

	return rc;

}

#endif

