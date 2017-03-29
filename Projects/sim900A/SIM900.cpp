#include "SIM900.h"

// define for WiringPi library
#ifdef WIRINGPI_LIB
#define pinMode_(pin, mode) pinMode(pin, mode)
#define pinWrite_(pin, level) digitalWrite(pin, level)
#define pinRead_(pin) digitalRead(pin)
#define delayUs_(time) delayMicroseconds(time)
#define delayMs_(time) delay(time)
#define OUTPUT_ OUTPUT
#define INPUT_ INPUT
#endif


SIM900::SIM900(uint8_t pwk_pin, uint8_t status_pin) {
	pwk_pin_ = pwk_pin;
	status_pin_ = status_pin;
	power_state_ = OFF;
	sim_card_ = false;
	ready_ = false;
};

SIM900::~SIM900() {}

bool SIM900::begin(char *path, uint32_t baudrate) {
	pinMode_(status_pin_, INPUT_);
	pinMode_(pwk_pin_, OUTPUT_);
	
	if((fd_uart_ = serialOpen(path, baudrate)) == -1 ) {
		return false;
	}

	restart();
	
 	if(sim_card_&&ready_)
 		return initMessage();
 	else
 		return false;
}

bool SIM900::switchPower(bool state) {
	if(state==power_state_)
		return power_state_;
	else {
		pinWrite_(pwk_pin_, HIGH);
		delayMs_(1500);
		pinWrite_(pwk_pin_, LOW);
		if(state==ON) {
			delayMs_(9000);
			std::string str = readLog();
			#if (DEBUG)
			std::cout << str << std::flush;
			#endif

			std::size_t found = str.find("+CPIN: READY");
			if (found!=std::string::npos)
		 		sim_card_ = true;

		 	found = str.find("Call Ready");
			if (found!=std::string::npos)
		 		ready_ = true;
		}
		else {
			delayMs_(2000);
			readLog();			// read power down meassage. Need this function if u wanna get right message at the next step
		}

		return powerStatus();
	}
}

bool SIM900::restart() {
	if(powerStatus()==ON)
		switchPower(OFF);

	switchPower(ON);
	return powerStatus();
}

bool SIM900::powerStatus() {
	if(status_pin_!= 255)
		power_state_ = pinRead_(status_pin_);
	else {
		serialPuts(fd_uart_, "AT\r\n");// The SMS text you want to send
		delayMs_(500);
		power_state_ = checkOk();
	}

	if(power_state_)
		return ON;
	else
		return OFF;
}

bool SIM900::call(const char *num) {
	serialPrintf(fd_uart_, "ATD%s;\r\n", num);
	return true;
}


void SIM900::close() {
	pinMode_(pwk_pin_, INPUT_);
	serialClose(fd_uart_);
}


uint8_t SIM900::readLog(char *s, uint8_t size) {
	int n = serialDataAvail(fd_uart_);
	int k = 0;
	for (int i = 0; i < n; i++) {
		char c = serialGetchar(fd_uart_);
		if(c!='\0') {
			s[k]=c;
			k++;
			if(k==size) break;
		}
	}
	s[k] = '\0';
	return k;
}

std::string SIM900::readLog() {
	int n = serialDataAvail(fd_uart_);
	std::string str;
	for (int i = 0; i < n; i++) {
		char c = serialGetchar(fd_uart_);
		if(c!='\0') {
			str +=c;
		}
	}
	// std::cout << str << std::flush;
	return str;
}

bool SIM900::initMessage() {
	serialPrintf(fd_uart_, "AT+CMGF=1\r\n");
	delayMs_(500);
	return checkOk();
}

bool SIM900::message(const char *num, const char* mesg) {
	serialPrintf(fd_uart_, "AT+CMGS=\"%s\"\r\n", num);
	delayMs_(1000);
  	serialPuts(fd_uart_, mesg);// The SMS text you want to send
  	delayMs_(100);
   serialPutchar(fd_uart_, (char)26);// ASCII code of CTRL+Z
  	delayMs_(1000);
  	return checkOk();
}

bool SIM900::checkOk() {
	std::string log = readLog();
	std::cout << log << std::flush;
	std::size_t found = log.find("OK");
	if (found!=std::string::npos)
		return true;
	else
		return false;
}

bool SIM900::setReadMessageImmediately() {
	serialPuts(fd_uart_, "AT+CNMI=2,2,0,0\r\n");
	delayMs_(500);
	return checkOk();
}

int8_t SIM900::readMessage(uint16_t pos, char *mesg, uint8_t size) {
	serialPrintf(fd_uart_, "AT+CMGR=%d\r\n", pos);
	delayMs_(1000);
	readLog(mesg, size);
	return 0;
}