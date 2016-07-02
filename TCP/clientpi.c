#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define maxBuffer   256
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[maxBuffer];
    // if (argc < 3) {
    //    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    //    exit(0);
    // }
    // portno = atoi(argv[2]);
    portno = 5010;

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        error("ERROR opening socket");

    // server = gethostbyname(argv[1]);
    server = gethostbyname("192.168.1.23");

    if (server == NULL) {
        error("ERROR, no such host");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    while(1){
        printf("Message: ");

        fgets(buffer,maxBuffer,stdin);
        printf("%lu\n",strlen(buffer));
        n = write(sockfd,buffer,strlen(buffer)-1);  // strlen(buffer)-1 : to ignore '\n' at last
        if (n < 0) 
            error("ERROR writing to socket");

        bzero(buffer,strlen(buffer));
    }

    close(sockfd);
    return 0;
}