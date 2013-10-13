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
