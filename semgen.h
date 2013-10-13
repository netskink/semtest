/*++
 *
 * Semgen.h program.
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

#ifdef movedtolinusr

#define SEMKEYPATH "/dev/null"  /* Path used on ftok for semget key  */
#define SEMKEYID 1              /* Id used on ftok for semget key    */

#define NUMSEMS 1               /* Num of sems in created sem set    */
#define NUMOPS 1               /* Num of operations in provided sem_unlock sem_lock things.  */
#define SIZEOFSHMSEG 50         /* Size of the shared mem segment    */


#define DONTCARE 0


extern struct sembuf sem_unlock;
extern struct sembuf sem_lock;
extern struct sembuf sem_lock_nowait;

//extern struct sembuf my_sem_operations[];

/* The set of semaphores.
 * 1  -- shared resource is available.
 * 0  -- shared resource is being used.
 */
//extern short  sarray[];

/* errno basic defines found here
 * /usr/include/asm/errno.h
 **/

#if 0
union semun {
	int val; 				/* value for SETVAL */
	struct semid_ds *buf; 	/* buffer for IPC_(STAT, SET) */
	u_short *array;			/* array for GETALL & SETALL */
}
#endif


#if 0
/* arg for semctl system calls. */
union semun {
        int val;                        /* value for SETVAL */
        struct semid_ds *buf;           /* buffer for IPC_STAT & IPC_SET */
        unsigned short *array;          /* array for GETALL & SETALL */
        struct seminfo *__buf;          /* buffer for IPC_INFO */
        void *__pad;
};
#endif

int mysemtimedwait(int semid);

#endif
