/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
// #include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXPENDING 5

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
    int servSock, clntSock, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t threadID;              /* Thread ID from pthread_create() */
    struct ThreadArgs *threadArgs;   /* Pointer to argument structure for thread */


    // if (argc < 2) {
    //     fprintf(stderr,"ERROR, no port provided\n");
    //     exit(1);
    // }

    // Create a socket
    if ( (servSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    // portno = atoi(argv[1]);
    portno = 5010;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind to the local address
    if ( bind(servSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR on binding");

    /* Mark the socket so it will listen for incoming connections */
    if ( listen(servSock, MAXPENDING) < 0)
        error("listen() failed");

    /* Set the size of the in-out parameter */
    clilen = sizeof(cli_addr);
    /* Wait for a client to connect */


    while(1) {

        if( (clntSock = accept(servSock, (struct sockaddr*) &cli_addr, &clilen)) < 0)
            error("accept() failed !");
        
        /* Create separate memory for client argument */
        if ((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
            error("malloc() failed");

        threadArgs -> clntSock = clntSock;

        if (pthread_create(&threadID, NULL, HandleClient, (void *) threadArgs) != 0)
            error("pthread_create() failed");

        // std::cout<<"+ New client["<<clntSock<<"]["<<inet_ntoa(cli_addr.sin_addr)<<"]["<<ntohs(cli_addr.sin_port)<<"]"<<"[Thread:"<<(long int) threadID<<"]\n";

        printf("\n+ New client[%d][Addr:%s][Port:%d][Thread:%ld]\n\n", 
            clntSock, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), (long int) threadID);
        // fflush(stdout);
    }

    // n = write(newsockfd,"I got your message",18);
    // if (n < 0) error("ERROR writing to socket");

    close(servSock);

    return 0; 
}

void *HandleClient(void *threadArgs){
    int n;
    int clntSock_thread;
    char buffer[256];
<<<<<<< HEAD:TCP/server-multask/server-thread.c
    bzero(buffer,256);
=======
    bzero(buffer, 256);
>>>>>>> c7ed500111d9db92879d688502f689bafce23ce3:TCP/server-multask/server-multitask.c
    clntSock_thread = ((struct ThreadArgs *) threadArgs) -> clntSock;
    while(1){
        n = read(clntSock_thread,buffer,255);
        // printf("%d\n",n);
        fflush(stdout);
        if (n < 0) 
            error("ERROR reading from socket");
        else if(n>0){
            printf(". Client[%d]: %s\n",clntSock_thread,buffer);
            fflush(stdout);
            bzero(buffer,strlen(buffer));
        }      
    }
}
