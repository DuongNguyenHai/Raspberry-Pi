// Nguyen Hai Duong
// simple server

#include <stdio.h>      // for printf() and fprintf()
#include <sys/socket.h> // for socket(), bind(), and connect()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for memset()
#include <unistd.h>     // for close()
#include <sys/types.h>  // for Socket data types
#include <netinet/in.h> // for IP Socket data types
#include <pthread.h>    // for multithread function

#define MAXPENDING 5
#define BUFFSIZE 256
#define PORT 8888

void *HandleClient(void *socket_desc);
struct ThreadArgs
{
    int clntSock;                      /* Socket descriptor for client */
};

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int servSock, clntSock;
    socklen_t clntLen;
    char buffer[BUFFSIZE];
    struct sockaddr_in serv_addr, cli_addr;

    pthread_t threadID;              /* Thread ID from pthread_create() */
    struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */

    // Create socket for incoming connections
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        error("ERROR opening socket");
      
    // Construct local address structure
    memset(&serv_addr, 0, sizeof(serv_addr));       /* Zero out structure */
    serv_addr.sin_family = AF_INET;                /* Internet address family */
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    serv_addr.sin_port = htons(PORT);              /* Local port */

    // Bind to the local address
    if (bind(servSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    // Mark the socket so it will listen for incoming connections 
    if (listen(servSock, MAXPENDING) < 0)
        error("ERROR on binding");

    // Set the size of the in-out parameter
    clntLen = sizeof(cli_addr);

    while(1){
        
        // Wait for a client to connect
        if( (clntSock = accept(servSock, (struct sockaddr*) &cli_addr, &clntLen)) < 0)
            error("accept() failed !");
        
        getpeername(clntSock, (struct sockaddr *) &cli_addr, &clntLen);

        /* Create separate memory for client argument */
        if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
            error("malloc() failed");

        threadArgs -> clntSock = clntSock;

        if (pthread_create(&threadID, NULL, HandleClient, (void *) threadArgs) != 0)
            error("pthread_create() failed");

        printf("\n+ New client[%d][Addr:%s][Port:%d]\n\n", 
            clntSock, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    }

    close(servSock);

    return 0; 
}

void *HandleClient(void *threadArgs){
    int clntSock;
    int recvMsgSize;
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);

    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    
    while(1){
        recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
        if (recvMsgSize < 0) 
            error("ERROR reading from socket");
        else if(recvMsgSize>0){
            printf(". Client[%d]: %s\n",clntSock,buffer);
            bzero(buffer,strlen(buffer));
        }
        else{
            printf("- Client[%d]: disconnected !\n",clntSock);
            break;
        }     
    }
    close(clntSock);
}