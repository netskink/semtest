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
	int nowait = 0;

	struct sembuf freesem;

	/* determine if this is a nowait or normal invocation. */
	if (argc > 2) {
		myprint("USAGE: %s [nowait]\n",argv[0]);
		my_exit(1);
	}
	
	if (argc > 1) {
		myprint("Assuming extra argument is a nowait specifier.\n");
		nowait = 1;
	}
	


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

	myprint("WFT is EAGAIN = %d \n",EAGAIN);
	if (nowait) {
		myprint("Doing sem lock no wait.\n");
		rc = semop(semid, &sem_lock_nowait, NUMOPS);
		myprint("semop rc = %d \n",rc);
	} else {
		myprint("Doing sem lock wait.\n");
		rc = semop(semid, &sem_lock, NUMOPS);
		myprint("semop rc = %d \n",rc);
	}
	if ( rc == 1 ) {
		myprint("lock failed.  my_get_last_error is %ld\n",status);
		my_exit(-1);
	} else if ( rc == EAGAIN ) {
		myprint("lock failed.  nowait specified and its already locked.\n");
	} else {
		myprint("locked worked.  We have the resource.\n");   
	}

	readline("hit return to continue\n");



	exit(0);
}

