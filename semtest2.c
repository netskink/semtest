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


	semid = semget ( semkey, 0, 0 );
	if ( semid == -1 ) {
		myprint("semget failed.  my_get_last_error is %ld\n",status);
		my_exit(-1);
	} else {
		myprint("semget on existing semaphore worked.\n");
	}


	/*
	 * On create we should set it used/0.  We are going to "use it" first.
	 * On open we don't dork with it.
	 * TODO: fix so its not always "freed"
	 */
//	sarray[0] = 1;
//	myprint("sarray[0] = %d\n",sarray[0]);
	/* int rc = semctl(int semid, int semnum, int cmd, union semun arg) */
	/* used to change permissions and other attributes of semaphore
	 * semnum, specifies a semaphore within an array by its index
	 */
	if ( semop(semid, &sem_lock, NUMOPS) == 1 ) {
		myprint("lock failed\n");
		my_exit(-1);
	} else {
		myprint("locked worked.  We have the resource.\n");
	}

	readline("hit return to continue. We will unlock it.\n");


	if ( semop(semid, &sem_unlock, NUMOPS) == 1 ) {
		myprint("unlock failed\n");
		my_exit(-1);
	} else {
		myprint("unlocked worked.  We have freed the resource.\n");
	}

	readline("hit return to continue. We will remove it.\n");



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

