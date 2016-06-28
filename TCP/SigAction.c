#include <stdio.h>  /* for printf() */
#include <signal.h> /* for sigaction() */
#include <unistd.h> /* for pause() */
#include <stdlib.h> /* for exit() */

#include "HandleTCPClient.c"
#include "DieWithError.c"

void DieWithError(char *errorMessage);       /* Error handling function */
void InterruptSignalHandler(int signalType); /* Interrupt signal handling function */

int main(int argc, char *argv[])
{
    struct sigaction handler;    /* Signal handler specification structure */

    /* Set InterruptSignalHandler() as handler function */
    handler.sa_handler =  InterruptSignalHandler;
    /* Create mask that mask all signals */
    if (sigfillset(&handler.sa_mask) < 0) 
        DieWithError("sigfillset() failed");
    /* No flags */
    handler.sa_flags = 0;

    /* Set signal handling for interrupt signals */
    if (sigaction(SIGINT, &handler, 0) < 0)
        DieWithError("sigaction() failed");

    for(;;)
        pause();  /* suspend program until signal received */


    exit(0);
}

void InterruptSignalHandler(int signalType)
{
    printf("Interrupt Received.  Exiting program.\n");
    exit(1);
}