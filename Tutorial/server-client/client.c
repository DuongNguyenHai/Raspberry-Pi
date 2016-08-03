// Nguyen Hai Duong
// simple client

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */

#define BUFFSIZE 256
#define PORT 8888

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){

    int sockfd, n;
    struct sockaddr_in serv_addr;
    char buffer[BUFFSIZE];
    char *servIP;
    if (argc < 2)
    {
        servIP = "127.0.0.1";   // localhost
    }else{
        servIP = argv[1];
    }

    if ((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        error("socket() failed");

    /* Ghi cau truc dia chi cho server */
    memset(&serv_addr, 0, sizeof(serv_addr));               /* Zero out structure */
    serv_addr.sin_family      = PF_INET;                    /* Internet address family */
    serv_addr.sin_addr.s_addr = inet_addr(servIP);          /* Server IP address */
    serv_addr.sin_port        = htons(PORT);                /* Server port */


    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    while(1){
        printf("message: ");
        bzero(buffer,BUFFSIZE);
        fgets(buffer,BUFFSIZE,stdin);
        if ( send(sockfd,buffer,strlen(buffer)-1,0) < 0) 
            error("ERROR writing to socket");
    }

    close(sockfd);
    return 0;
}