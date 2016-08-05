// Nguyen Hai Duong
// simple server

#include <stdio.h>          /* for printf() and fprintf() */
#include <sys/socket.h>     /* for socket(), connect(), send(), and recv() */
#include <stdlib.h>         /* for atoi() and exit() */
#include <string.h>         /* for memset() */
#include <unistd.h>         /* for close() */
#include <sys/types.h> 
#include <netinet/in.h>

#define MAXPENDING 5
#define BUFFSIZE 256
#define PORT 8888

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int servSock, clntSock, n;
    socklen_t clntLen;
    char buffer[BUFFSIZE];
    struct sockaddr_in serv_addr, cli_addr;

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
    
    // Wait for a client to connect
    if ((clntSock = accept(servSock, (struct sockaddr *) &cli_addr, &clntLen)) < 0)
        error("accept() failed");

    bzero(buffer,BUFFSIZE);

    while(1){
        n = recv(clntSock,buffer,BUFFSIZE,0);
        if ( n<0 ) 
            error("ERROR reading from socket");
        else if( n>0 ){
            printf("Here is the message: %s\n",buffer);
            fflush(stdout);
            bzero(buffer,BUFFSIZE);
        }
        else{
            printf("Client disconnect !\n");
            break;
        }
    }

    close(servSock);

    return 0; 
}