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
/* This program creates a semaphore, and inits it.  It will write ready to screen
 * and free the semaphore.  Then it will wait 3 seconds.  If the semaphore is still ready,
 * it will take the sema
 *
 * This program is a system V semaphore test program.
 *
 * It creates a semaphore and initializes it to be used.  If the semaphore already exists, it will close it
 * and create it fresh.
 *
 * Once the semaphore is created, it will take the semaphore by setting it to one.  It will print a message
 * I have the resource and it will sleep for 1 second.  Then it will free the semaphore.  It will sleep again
 * for one second and see if it can get the semaphore.  It will wait 5 seconds to get it.  If it can not get it,
 * it destory the semaphore and exit.  If it can get the semaphore, it will loop until it has aquired the semaphore 
 * ten times.
 *
 */

my_main(argc,argv[]) {


	
	key_t semkey;
	int rc, semid, i;
	int status;

	struct sembuf freesem;
	union semun arg;




	semkey = ftok(SEMKEYPATH,SEMKEYID);
	if ( semkey == (key_t)  -1 ) {
		myprint("ftok() for sem failure.\n");
		my_exit(-1);
	}

	myprint("ipcs command will list the IPCs currently in use on the system.\n");
	myprint("ipcrm command will remove a specified semaphore.\n");
	myprint("-------\n");
	myprint("semkey = 0x%x\n",semkey);
	myprint("SEMKEYPATH = %s\n",SEMKEYPATH);
	myprint("SEMKEYID = %d\n",SEMKEYID);
	myprint("NUMSEMS = %d\n",NUMSEMS);

	/* int semid = semget (key_t key, int nsems, int semflg) */ 
	semid = semget ( semkey, NUMSEMS, 0777 | IPC_CREAT | IPC_EXCL );
	if ( semid == -1 ) {
		myprint("semget() failed.  Does is already exist?\n");
	
		status = my_get_last_error();
		myprint("semget failed.  my_get_last_error is %ld\n",status);
		if (EEXIST == status) {
			// sem already exists.  Must reopen to remove it.
			myprint("semget failed.  sem already exists.\n");
			myprint("It seems we need to get it again, but witout the create flag.\n");
			semid = semget ( semkey, 0, 0 );
			if ( semid == -1 ) {
				myprint("rm sem failed\n");
				status = my_get_last_error();
				myprint("semget failed.  my_get_last_error is %ld\n",status);
				my_exit(-1);
			} else {
				myprint("semget on existing semaphore worked.\n");
			}

			myprint("Now try to remove sem\n");
			/* int rc = semctl(int semid, int semnum, int cmd, union semun arg) */
			/* used to change permissions and other attributes of semaphore
			 * semnum, specifies a semaphore within an array by its index
			 */
			status = semctl(semid, 0, IPC_RMID);
			if ( status == -1 ) {
				myprint("rm sem failed\n");
				status = my_get_last_error();
				myprint("semget failed.  my_get_last_error is %ld\n",status);
				my_exit(-1);
			}
			myprint("remove sem worked\n");
			exit(0);
		} 

	} else {
		myprint("Fresh create of semaphore passed.\n");
		myprint(" key=0x%d\n",semkey);
		myprint(" id=%d\n",semid);
	}

	/*
	 * After you create a semaphore, you must initialize it.  In this case,
	 * the semaphore is intialized to be available/free.
	 */
	arg.val = 1; 		/* value for SETVAL */
	status = semctl(semid, 0, SETVAL, arg);
	if ( status != 0  ) {
		status = my_get_last_error();
		myprint("sem initalize failed.  my_get_last_error is %ld\n",status);
		my_exit(-1);
	}



	/*
	 * Lock the semaphore
	 */
	if ( semop(semid, &sem_lock, NUMOPS) == 1 ) {
		myprint("lock failed\n");
		my_exit(-1);
	} else {
		myprint("locked worked.  We have the resource.\n");
	}

	readline("hit return to continue. We will unlock it.\n");


	/*
	 * Unock the semaphore
	 */
	if ( semop(semid, &sem_unlock, NUMOPS) == 1 ) {
		myprint("unlock failed\n");
		my_exit(-1);
	} else {
		myprint("unlocked worked.  We have freed the resource.\n");
	}

	readline("hit return to continue. \n");
	readline("We will attempt to lock.  If we can not lock within 10 secs we pass.\n");

#if 0

	/*
	 * Lock the semaphore no wait style
	 */
	if ( semop(semid, &sem_lock_nowait, NUMOPS) == 1 ) {
		myprint("lock failed\n");
		my_exit(-1);
	} else {
		myprint("locked worked.  We have the resource.\n");
	}

	readline("hit return to continue. We will remove it.\n");
#endif

// this routine was replaced with my_sem_wait	rc = mysemtimedwait(semid);
	rc = my_sem_wait(semid, 10 /* secs */);
	myprint("my sem timed wait returned %d\n",rc);


	/*
	 * Normal close
	 * remove the semaphore
	 */
	myprint("Try to remove sem\n");
	status = semctl(semid, 0, IPC_RMID);
	if ( status == -1 ) {
		myprint("rm sem failed\n");
		my_exit(-1);
	}
	myprint("remove sem worked\n");

	exit(0);
}

