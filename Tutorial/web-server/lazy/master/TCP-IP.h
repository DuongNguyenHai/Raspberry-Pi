#include <stdio.h>      // for printf() and fprintf()
#include <sys/socket.h> // for socket(), bind(), and connect()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>		// for memset()
#include <unistd.h>		// for close()
#include <sys/types.h>	// for Socket data types
#include <netinet/in.h> // for IP Socket data types
#include <pthread.h>	// for multithread function


#define MAXPENDING 5
#define BUFFSIZE 256
#define PORT 8888


// Function : creat tco server socket
int CreateTCPServerSocket(unsigned short port);

// Function : accept tcp connection
// Return server socket
int AcceptTCPConnection(int servSock);



// Show error message
void error(const char *msg);