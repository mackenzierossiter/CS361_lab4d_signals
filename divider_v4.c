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
#include <sys/msg.h>
#include <sys/stat.h>

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
    int shmMailbox = msgget(shmKey, msgflags);
    // int shmMailbox = msgget(shmKey, msgflg);
    if (shmMailbox == -1) {
        printf("Error with msgget");
        perror("Reason");
        raise(SIGSTOP);
    }
    shmStruct.ratio = shmStruct.num1 / shmStruct.num2;

    return 0;
}
