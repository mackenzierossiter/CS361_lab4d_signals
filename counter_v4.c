/*-------------------------------------------------------------------------------
IPC using Shared Memory & Signals -- Counter Version 00
Written By: 
     1- Dr. Mohamed Aboutabl
-------------------------------------------------------------------------------*/

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
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

bool FOREVER = true ;

//------------------------------------------------------------
/* Wrapper for sigaction */

typedef void Sigfunc( int ) ;

Sigfunc * sigactionWrapper( int signo, Sigfunc *func )
// 'signo' specifies the signal and can be any valid signal 
//  except SIGKILL and SIGSTOP.

{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset( &act.sa_mask );
	act.sa_flags   = 0;

	if( sigaction( signo, &act, &oact ) < 0 )
		return( SIG_ERR );

	return( oact.sa_handler );
}

//------------------------------------------------------------
void sigHandler_A( int sig ) 
{
    fflush( stdout ) ;
    printf("\n\n### I (%d) received Signal #%3d.\n\n"
           , getpid() , sig );  
    // raise(SIGSTOP);
    // printf("Here before fork()\n");
    // fflush( stdout ) ;
    pid_t child_pid = fork();
    if (child_pid < 0) {
        raise(SIGSTOP);
    } if (child_pid == 0) {
        // printf("HERE");
        // fflush( stdout ) ;
        if (execl("./divider", "", NULL) < 0) 
        {
            perror("Error in execl of driver process");
        }
    }
    
}


//------------------------------------------------------------
void sigHandler_CONT( int sig ) 
{
    fflush( stdout ) ;
    
    printf("\n\n### I (%d) have been asked to RESUME by Signal #%3d.\n\n"
           , getpid() , sig );  
    FOREVER = false;
    
}

//------------------------------------------------------------

int main(int argc , char * argv[])
{
    shmData shmStruct;
    pid_t  mypid ;

    mypid = getpid() ;
    printf("\nHELLO! I AM THE COUNT PROCESS WITH ID= %d\n" , mypid );
      
    // Set up Signal Catching here
    sigactionWrapper(SIGCONT, sigHandler_CONT);
    sigactionWrapper(SIGTSTP, sigHandler_A);
    // sigactionWrapper(SIGSTOP, sigHandler_B);
    int msgflg  =  IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IWOTH | S_IWGRP | S_IRGRP | S_IROTH; // add RGRP ROTH
    key_t shmKey = ftok("shmSegment.h", 1);
    if (shmKey == -1) {
        printf("Error with ftok");
        perror("Reason");
        exit(EXIT_FAILURE);
        // raise(SIGSTOP);
    }

    // int msgflags = S_IWUSR;
    int shmMailbox = msgget(shmKey, msgflg);
    if (shmMailbox == -1) {
        printf("Error with msgget");
        perror("Reason");
        exit(EXIT_FAILURE);
        // raise(SIGSTOP);
    }
    

    printf("HERE before .num1");
    shmStruct.num1 = atoi(argv[1]);
    shmStruct.num2 = atoi(argv[2]);

    unsigned i=0;
    while(  FOREVER ) {
        printf("%10X\r" , i++ ) ;
        printf("%f", shmStruct.ratio);

    }
        
    printf("\nCOUNTER: Stopped Counting. The FOREVER flag must have become FALSE\n\n");
    printf("\nCOUNTER: Goodbye\n\n");
    
	return 0 ;

}
