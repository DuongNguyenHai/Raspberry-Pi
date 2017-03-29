// Compile : g++ -Wall -std=c++11 main.cc SIM900.cpp -o main -lwiringPi

#include <stdio.h>
#include <errno.h>
#include "SIM900.h"

#define PWK_PIN 4
#define STATUS_PIN 27

void sig_handler(int signo);

SIM900 myphone(PWK_PIN);

int main() {

	wiringPiSetupGpio();
	
	myphone.begin();

	// printf("res: %s\n", (res?"YES":"NO"));
	// printf("sim: %s\n", (myphone.hasSim()?"YES":"NO"));
	// printf("ready: %s\n", (myphone.ready()?"YES":"NO"));

	// myphone.call("01235066750");

	// myphone.message("0962539275", "hello Thuy !!!");
	myphone.setReadMessageImmediately();
	// delay(10000);
	// char s[100];

	// myphone.readMessage(0, s, 100);

	while(1) {
		if (serialDataAvail(myphone.fd_uart_)) {
			char c = serialGetchar(myphone.fd_uart_);
			if(c!='\0') {
				printf("%c", c);
				fflush (stdout);
			}
		}
	}

	return 0;
}

void sig_handler(int signo) {
	if (signo == SIGINT) {
		// cleanup port here
		myphone.close();
		exit(1);
	}
}