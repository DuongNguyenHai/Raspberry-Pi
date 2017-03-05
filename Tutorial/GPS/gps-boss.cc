// Nguyen Hai Duong
// Jun 01 2016
// Module GPS L70-R

// compile : g++ -std=c++11 -Wall gps-boss.cc -o gps-boss -lwiringPi

#include <stdio.h>
#include <iostream>
#include <string>
#include <errno.h>      // perror function to print errno message
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close(), sleep */
#include <sys/wait.h>     // wait()
#include <wiringPi.h>   // wiringPi function
#include <wiringSerial.h>  // serial wiringpi

using namespace std;

#define GOOGLE_MAP_IP "172.217.4.170"
#define BUFFSIZE 1024
#define PORT 80

#define GET_BEGIN "GET /maps/api/geocode/json?latlng="
#define GET_END " HTTP/1.1\r\nHost: maps.googleapis.com\r\nConnection: close\r\n\r\n" // space at the fist of string really important

int fd;              // file descritor for serial port
char *cmd = (char *)"$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
// char *rs = (char *)"$GPRMC,104732.000,A,2046.9864,N,10643.3912,E,0.09,254.71,301216,,,A*6C"; // use it to test HandleGPRMC() function

// array of type of respond, look at respond type in GPS l70-R datasheet
string header[6] = {"GPRMC", "GPVTG", "GPGGA", "GPGSA", "GPGSV", "GPGLL"};

enum type_respond {
   GPRMC = 0,
   GPVTG = 1,
   GPGGA = 2,
   GPGSA = 3,
   GPGSV = 4,
   GPGLL = 5
} Type;

struct type_time {
   int hour;
   int minute;
   int second;
   int year;
   int month;
   int day;
} date_time;

struct type_location {
   string latitude;
   string longitude;
   string location;
} position;

string GetMessage();
int ClassifyRespond(string str);             // classify type of message responded from GPS device
int HandleGPRMC(string str);                 // handle with GPRMC message
int CheckSum(const char *s);                 // check sum
int GetDateTime(string time, string date);   // get date and time from message
string ConvertCoordinate(string coor);       // convert DDMM.MM coordinate to DD coordinate (its suitable with google map)
int GetLocation(string content);             // get location from google map
int PrintCoordinate();                       // print coordinate
int PrintLocation();                         // print location got from google map
int PrintDateTime();                         // print date and time

int Tracking();                              // user can ask to do something

int main() {

   while(1) {
      if((fd = serialOpen ("/dev/ttyUSB0", 9600)) < 0 ) {
         fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
         sleep(1);
      } else break;
   }
   
   printf("GPS is working ");
   // we will wait until GPS has read data successfully
   while(1) {
      printf("."); fflush(stdout);
      serialPuts(fd, cmd);    // send command
      serialFlush(fd);     // clear all old data in buffer
      string message = GetMessage();
      
      int typeCmd = ClassifyRespond(message);
      if(typeCmd==GPRMC) { 
         if(HandleGPRMC(message)==0)   // its mean the message is valid and GPS has read data successfully
            break;
      }
      sleep(1);
   }

   printf("\nGPS is already !\n-----------------\n");

   while(1) {

      int userCmd = Tracking();

      while(1) {
         serialFlush(fd);     // clear all old data in buffer
         string message = GetMessage();
         int typeCmd = ClassifyRespond(message);
         if(typeCmd==GPRMC) { 
            if(HandleGPRMC(message)==0) {
               GetLocation(position.latitude + ',' + position.longitude);
               break;
            }
         }
      }

      switch (userCmd) {
         case 0: {
            printf(":: ");
            PrintDateTime();
         } break;
         case 1: {
            printf(":: ");
            PrintLocation();
         } break;
         default:;
      }
   }

   return 0;

}

string GetMessage() {
   int n = 0;
   char c;
   char raw[256];
   while(1) {
      if(serialDataAvail(fd)) {
         c = serialGetchar(fd);
         if(c=='\r') {
            break;
         }
         raw[n] = c;
         n++;
      }  
   }
   raw[n] = '\0';
   string message(raw);
   return message;
}

int ClassifyRespond(string str) {

   if( str.size()==0 || str.at(0)!='$') {
      // printf("Respond from GPS is not valid !\n");
      return -1;
   }
   size_t found = str.find(',');
   if (found==std::string::npos) {
      // printf("not found command in message\n");
      return -1;
   }
   string hd = str.substr(1, found-1).c_str();
   hd[found-1] = '\0';
   for(unsigned int i=0; i<sizeof(header)/sizeof(header[0]); i++) {
      if(hd.compare(header[i])==0)
         return (int)i;
   }
   return -1;
}

int HandleGPRMC(string str) {

   // Find out in GPS L70-R datasheet. see structure of respond message in GPRMC table. The commands below will spilit content between two ','
   int GPRMC_ELEMENT = 12;
   string GPRMC_field[GPRMC_ELEMENT];

   size_t head = str.find(',');
   GPRMC_field[0] = str.substr(1, head-1);
   // spilit data between two ','
   for (int i = 1; i < GPRMC_ELEMENT-2; ++i) {
      size_t tail = str.find(",", head+1);
      GPRMC_field[i] = str.substr(head+1, tail - (head+1));
      if(GPRMC_field[2].compare("V")==0) {
         // printf("The GPRMC message is not valid\n");
         return -1;
      }
      head = tail;
   }
   
   head = str.find('*');
   GPRMC_field[(GPRMC_ELEMENT-2)] = str.substr(head-1, 1);
   GPRMC_field[(GPRMC_ELEMENT-2)+1] = str.substr(head+1, 2);
   
   // for (int i = 0; i < GPRMC_ELEMENT; ++i)
   //    printf("GPRMC_field[%d]: %s\n", i, GPRMC_field[i].c_str());

   string ss = str.substr(1, head-1);
   // printf("ss: %s\n", ss.c_str());
   int check = CheckSum((char *)ss.c_str());
   // printf("checksum (int) %d, (hex) 0x%02X\n", check, check);

   int orign_sum = (int)strtol(GPRMC_field[(GPRMC_ELEMENT-2)+1].c_str(), NULL, 16);
   // printf("orign_sum %d\n", orign_sum);
   if(check!=orign_sum) {
      // printf("checksum is wrong. Data is not valide\n");
      return -1;
   }

   position.latitude = ConvertCoordinate(GPRMC_field[3]);
   position.longitude = ConvertCoordinate(GPRMC_field[5]);
   GetDateTime(GPRMC_field[1], GPRMC_field[9]);

   return 0;

}

int GetDateTime(string time, string date) {


  string dt = time.substr(0,2);    // get 2 character from index 0
  date_time.hour = stoi(dt);

  dt = time.substr(2,2);       // get 2 character from index 2
  date_time.minute = stoi(dt); // convert to int

  dt = time.substr(4,2);
  date_time.second = stoi(dt);

  dt = date.substr(0,2);
  date_time.day = stoi(dt);

  dt = date.substr(2,2);
  date_time.month = stoi(dt);

  dt = date.substr(4,2);
  date_time.year = stoi(dt) + 2000;    // add + XX century

  // printf("UTC time %d:%d%d, date %d:%d:%d\n", date_time.hour, date_time.minute, date_time.second, date_time.day, date_time.month, date_time.year);

  return 0;
}

int GetLocation(string content) {
   int sockfd, n;
   struct sockaddr_in serv_addr;
   char buffer[BUFFSIZE];

   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
      perror("socket() failed");

   /* Adding structer for server */
   memset(&serv_addr, 0, sizeof(serv_addr));               /* Zero out structure */
   serv_addr.sin_family      = AF_INET;                    /* Internet address family */
   serv_addr.sin_addr.s_addr = inet_addr(GOOGLE_MAP_IP);       /* Server IP address */
   serv_addr.sin_port        = htons(PORT);                /* Server port */

   if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
      perror("ERROR connecting");

   // printf("content: %s\n", content);
   // send get method : content include latitude and longtitude
   if (  send(sockfd, GET_BEGIN, strlen(GET_BEGIN),0) < 0 ||
         send(sockfd, content.c_str(), content.length(),0) < 0 ||
         send(sockfd, GET_END, strlen(GET_END),0) < 0 ) {
            perror("ERROR writing to socket");
   }

   // get json respond from google map API

   while(1){
      n = recv(sockfd, buffer, BUFFSIZE,0);

      if ( n<0 ) 
         perror("ERROR reading from socket");
      else if( n>0 ){
         // printf("%s",buffer);       // print respond from google.
         string respondGg(buffer);
         size_t found = respondGg.find("formatted_address");
         if (found!=std::string::npos) {
            size_t head = respondGg.find(':', found);
            size_t tail = respondGg.find('\n', head);
            if (tail==std::string::npos) {
               printf("ERROR respond from google\n");
            }

            position.location = respondGg.substr(head + 3 , tail - (head + 3) - 2);
            // printf("%s\n", position.location.c_str());
            break;
         }

         bzero(buffer, BUFFSIZE);
        
      } else break;
   }

   close(sockfd);
   return 0;
}

string ConvertCoordinate(string coor) {
   
   size_t found = coor.find('.');
   string degree = coor.substr(0, found - 2 );
   string minute = coor.substr(found-2, coor.length()-(found-2));

   float dd = stof(degree) + stof(minute)/60;
   // printf("degree: %s, minute: %s, decimal: %s\n", degree.c_str(), minute.c_str(), to_string(dd).c_str());
   
   return to_string(dd);
}

int CheckSum(const char *s) {
   int c = 0;

   while(*s)
      c ^= *s++;

   return c;
}

int Tracking() {
   string str;
   while(1) {
      printf(":: ");
      getline(cin, str);
      if(str.find("whats time")==0) {
         return 0;
      }
      else if(str.find("where is this")==0) {
         return 1;
      }
   }
   return 0;
}

int PrintCoordinate() {
   printf("Coordinate latitude : %s, longitude : %s\n", position.latitude.c_str(), position.longitude.c_str());
   return 0;
}

int PrintDateTime() {
   printf("UTC time %d:%d:%d, date %d:%d:%d \n",date_time.hour,date_time.minute, date_time.second,date_time.day,date_time.month,date_time.year);
   return 0;
}

int PrintLocation() {
   printf("Position: %s\n", position.location.c_str());
   return 0;
}
