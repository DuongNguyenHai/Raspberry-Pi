

#ifndef SIM900_H
#define SIM900_H

#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <string> 
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define WIRINGPI_LIB

#define pathUART (char*)"/dev/ttyAMA0"
#define ON true
#define OFF false

#define DEBUG true

class SIM900 {

public:
	SIM900(uint8_t pwk_pin, uint8_t status_pin = 255);
	~SIM900();
	bool begin(char *path = pathUART, uint32_t baudrate = 9600);
	bool switchPower(bool state);
	bool restart();
	bool powerStatus();
	bool call(const char *num);
	bool message(const char *num, const char* mesg);
	int8_t readMessage(uint16_t pos, char *mesg);
	bool hasSim() { return sim_card_;}
	bool ready() { return ready_;};
	int8_t readMessage(uint16_t pos, char *mesg, uint8_t size);
	bool initMessage();
	bool setReadMessageImmediately();
	uint8_t readLog(char *s, uint8_t size);
	std::string readLog();
	void close();
	int16_t fd_uart_;											// file descriptor for uart serial
	
private:
	uint8_t pwk_pin_;											// connect to pwk pin
	uint8_t status_pin_;										// connect to status_pin
	bool power_state_;									// status of power
	bool sim_card_;
	bool ready_;
	bool checkOk();
};





#endif