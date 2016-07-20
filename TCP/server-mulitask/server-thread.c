/*
    Nguyen Hai Duong
    Date : 11 jun 2016

    compile : gcc -o server-thread server-thread.c -lpthread
*/
#include "TCP-IP.h"
#include "TCP-IP.c"

int main(int argc, char *argv[]){

    int servSock, clntSock;
    struct sockaddr_in cli_addr;
    pthread_t threadID;              /* Thread ID from pthread_create() */
    struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */
    unsigned int clntLen;            /* Length of client address data structure */

    clntLen = sizeof(cli_addr);

    servSock = CreateTCPServerSocket(PORT);

    while(1) {

        clntSock = AcceptTCPConnection(servSock);

        getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);

        /* Create separate memory for client argument */
        if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
            error("malloc() failed");

        threadArgs -> clntSock = clntSock;

        if (pthread_create(&threadID, NULL, HandleThreadClient, (void *) threadArgs) != 0)
            error("pthread_create() failed");

        printf("\n+ New client[%d][Addr:%s][Port:%d]\n\n", 
            clntSock, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    }

    close(servSock);
    return 0; 
}