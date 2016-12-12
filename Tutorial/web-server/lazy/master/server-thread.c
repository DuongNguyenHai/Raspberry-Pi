/*
    Nguyen Hai Duong
    Date : 11 jun 2016

    compile : gcc -std=c11 -Wall -g -D_BSD_SOURCE server-thread.c -o server-thread -lpthread
*/
#include <string.h>
#include <strings.h>
#include "TCP-IP.h"
#include "TCP-IP.c"
#include <stdbool.h>    // for bool type
#include <stdlib.h> // for itoa() call

#define localhost "127.0.0.1"
#define SET "OK"

char *IdDevice[4][3] = { {"Living Room Light", "L1", NULL},
                    {"Toilet Light", "L2", NULL},
                    {"Bed Room Light", "L3", NULL},
                    {"Font Door", "D1", NULL},
};

void ServerCommand(char *str);
bool IdentifyDevice(int clntSock, char *str);
// Function : handle client
void *HandleThreadClient(void *socket_desc);
// send a command to device 
bool SendCommandToDevice(int index, char *str);
// sizeof(IdDevice)/sizeof(IdDevice[0][0])/2 : the number of row
#define SIZE_OF_ARRY2(array2) (sizeof(array2)/sizeof(array2[0][0])/3) 

int servSock;   // global variable

int main(int argc, char *argv[]){

    int clntSock;
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
        threadArgs -> addr = inet_ntoa(cli_addr.sin_addr);

        if (pthread_create(&threadID, NULL, HandleThreadClient, (void *) threadArgs) != 0)
            error("pthread_create() failed");

        // printf("\n+ New client[%d][Addr:%s]\n\n", 
        //     clntSock, inet_ntoa(cli_addr.sin_addr));
    }

    close(servSock);
    return 0; 
}

void *HandleThreadClient(void *threadArgs){
    int clntSock;
    int recvMsgSize;
    char buffer[BUFFSIZE];
    bzero(buffer,BUFFSIZE);

    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    
    while(1){
        recvMsgSize = recv(clntSock,buffer,BUFFSIZE,0);
        if (recvMsgSize < 0) {
            printf("ERROR reading from socket\n");
        }
        else if(recvMsgSize>0){
            
            if( !strcmp(((struct ThreadArgs *) threadArgs) -> addr, localhost) ) { // if data from server   
                printf(". Web server: %s\n", buffer);
                ServerCommand(buffer);
            }else {
                printf(". Device: %s\n", buffer);
                // Check command of device
                char *header = strtok(buffer, ":");
    			char *content = strtok(NULL, ":");
                if(strcmp(header,"RESULT")==0) { // result command
                    // response from device
                    printf(". Address[%s]: %s\n", ((struct ThreadArgs *) threadArgs) -> addr, content);
                } else if (strcmp(header,"INIT")==0) {    // Init a new device command
                    IdentifyDevice(clntSock, content);
                } else {
                    // other command
                }
            }
            bzero(buffer,strlen(buffer));
        }
        else {
            if( !strcmp(((struct ThreadArgs *) threadArgs) -> addr, localhost) ) {
                // printf("- Web server disconnected\n");
            }
            else {
                printf("- Address[%s]: disconnected !\n", ((struct ThreadArgs *) threadArgs) -> addr);
                // u can handle with client-disconnected event
                // code here
            }
            
            break;
        }     
    }
    close(clntSock);
}

void ServerCommand(char *str) {

    char *header = strtok(str, ":");
    char *content = strtok(NULL, ":");
    for (int i = 0; i < SIZE_OF_ARRY2(IdDevice); ++i) { 
        if(IdDevice[i][0]) // check if IdDevice[i][0] is not NULL
            if(strcmp(header, IdDevice[i][0])==0) {
                // printf("header: %s, IdDevice: %s, content: %s\n", header, IdDevice[i][1], content);
                if(IdDevice[i][2]!=NULL)    // check if has device
                    SendCommandToDevice(i, content);
                else
                    printf("- There is no device: \"%s\"\n", IdDevice[i][0]);
                // break;       // break when u just wanna send to one device
            }
    }
}

bool IdentifyDevice(int clntSock, char *str) {

    for (int i = 0; i < SIZE_OF_ARRY2(IdDevice); ++i) { 
        if(IdDevice[i][0]) // check if IdDevice[i][0] is not NULL
            if(strcmp(str, IdDevice[i][1])==0) {
                printf("+ Detecting a new device: %s, ID:%s\n\n", IdDevice[i][0], IdDevice[i][1]);
                if ( send(clntSock, SET,strlen(SET),0) < 0) { // send "OK"
                    printf("- Sending \"OK\" to device: \"%s\" false\n", IdDevice[i][0]);
                    return false;
                }
                char clientAddr[4]; // save client socket as string, max is "9999"
                sprintf(clientAddr, "%d", clntSock);    // convert to string
                IdDevice[i][2] = strdup(clientAddr);    // save to IdDevice
                return true;
            }
    }
    return false;
}

bool SendCommandToDevice(int index, char *str) {

    int clntSock = atoi(IdDevice[index][2]);
    if ( send(clntSock, str,strlen(str),0) < 0) {
        printf("- Sending to device: \"%s\" failed\n", IdDevice[index][0]);
        return false;
    }
    printf(". Sending to device: \"%s\", content: %s\n", IdDevice[index][0], str);

    return false;
}
