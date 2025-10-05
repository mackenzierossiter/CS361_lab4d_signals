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
    shmData shmStruct;
    pid_t  mypid      = getpid() ;

    printf("\n\nHELLO! I am the newly-born CHILD ID= %d\n" , mypid );


    // Awaken my Parent
    pid_t parentID = getpgid(mypid);
    kill(SIGCONT, parentID);

    int msgflg  =  S_IRUSR | S_IWUSR | S_IWOTH | S_IWGRP | S_IRGRP | S_IROTH; // add RGRP ROTH
    key_t shmKey = ftok("shmSegment.h", 1);
    if (shmKey == -1) {
        printf("Error with ftok");
        perror("Reason");
        raise(SIGSTOP);
    }
    int msgflags = S_IWUSR;
    int shmMailbox = shmget(shmKey, SHMEM_SIZE, msgflags);
    // int shmMailbox = msgget(shmKey, msgflg);
    if (shmMailbox == -1) {
        printf("Error with shmget");
        perror("Reason");
        raise(SIGSTOP);
    }
    printf("line 58\n");
    char * sharedMem = shmat(shmMailbox, NULL, 0);
    if (sharedMem < 0) {
        printf("shmat failed\n");
        perror("Reason");
        exit(EXIT_FAILURE);
    }
    printf("Divider: Dividing \t%d by \t%d", sharedMem.num1, shmStruct.num2);
    sharedMem.ratio = sharedMem.num1 / sharedMem.num2;

    return 0;
}
