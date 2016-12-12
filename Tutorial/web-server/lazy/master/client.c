// Nguyen Hai Duong
// simple client
// notice that client has the same ip with server. so its correspond to web-server.
// To test it lets change 

#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <stdbool.h>    // for bool type

#define BUFFSIZE 256
#define PORT 8888

#define DEVICE_NAME "Living Room Light"
#define IdDevice "L1"
#define INIT_IdDevice "INIT:"IdDevice
#define SET "OK"

int sockfd;
int recvMsgSize;
char buffer[BUFFSIZE];
bool SendToMaster(char *str);

void error(const char *msg){
    perror(msg);
    exit(0);
}

bool SentID();

int main(int argc, char *argv[]){

    struct sockaddr_in serv_addr;

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

    SentID();

    while(1) {
        
        recvMsgSize = recv(sockfd,buffer,BUFFSIZE,0);
        if (recvMsgSize==-1) {
            printf("ERROR reading from socket\n");
        }
        else if(recvMsgSize>0) {
            printf("Receive: %s\n", buffer);
            bzero(buffer,BUFFSIZE);
        }
    }

    close(sockfd);
    return 0;
}

bool SentID() {
    printf("Send identify : \"Living Room Light : L1\"\n");
    SendToMaster(INIT_IdDevice);

    bzero(buffer,BUFFSIZE);
    recvMsgSize = recv(sockfd,buffer,BUFFSIZE,0);
    if (recvMsgSize ==-1) {
        printf("ERROR reading from socket\n");
    }
    else if(recvMsgSize>0) {
        if(strcmp(buffer, SET)==0) {
            printf("the device was set: %s\n", buffer);
            return true;
        }
    } else {
        // something wrong
    }
    sleep(0.5);
    return false;
}

bool SendToMaster(char *str) {

    if ( send(sockfd, str, strlen(str),0) < 0) {
        printf("Error seding to master\n");
        return false;
    }

    return false;
}