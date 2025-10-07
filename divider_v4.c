/*-------------------------------------------------------------------------------
IPC using Shared Memory & Signals Divider Version 00
Written By: 
     1- Dr. Mohamed Aboutabl
-------------------------------------------------------------------------------*/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>



#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shmSegment.h"

int main()
{
    shmData *p;
    pid_t  mypid      = getpid() ;

    printf("\n\nHELLO! I am the newly-born DIVIDER with PID= %d\n" , mypid );

    pid_t parentID = getppid();
    //kill(parentID, SIGCONT);

    int msgflg  =  S_IRUSR | S_IWUSR | S_IWOTH | S_IWGRP | S_IRGRP | S_IROTH; // add RGRP ROTH
    key_t shmKey = ftok("shmSegment.h", 1);
    if (shmKey == -1) {
        printf("Error with ftok");
        perror("Reason");
        raise(SIGSTOP);
    }

    int shmMailbox = shmget(shmKey, SHMEM_SIZE, msgflg);
    // int shmMailbox = msgget(shmKey, msgflg);
    if (shmMailbox == -1) {
        printf("Error with shmget");
        perror("Reason");
        raise(SIGSTOP);
    }

    p = shmat(shmMailbox, NULL, 0);
    if (p < 0) {
        printf("shmat failed\n");
        perror("Reason");
        exit(EXIT_FAILURE);
    }
    printf("\nDIVIDER: Dividing \t%d by \t%d\n", p->num1, p->num2);
    p->ratio = (double) p->num1 / p->num2;

    printf("\nDIVIDER: will signal my parent to CONTINUE\n");
    //pid_t parentID = getppid();
    kill(parentID, SIGCONT);

    shmdt(p);
    return 0;
}
