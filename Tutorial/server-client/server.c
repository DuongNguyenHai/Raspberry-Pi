// Nguyen Hai Duong
// Date : 11 jun 2016

#include "TCP-IP.h"
#include "TCP-IP.c"
#include <sys/wait.h>/* for waitpid() */

int main(int argc, char const *argv[])
{
	int servSock, clntSock;
    int n;
    char buffer[BUFFSIZE];
    servSock = CreateTCPServerSocket(PORT);
    clntSock = AcceptTCPConnection(servSock);

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

	return 0;
}
