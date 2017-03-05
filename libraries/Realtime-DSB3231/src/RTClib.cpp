// Author : Nguyen Hai Duong
// Jan 2017
// Part of library by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

// Version 1.0

// Modify date : Feb 20 2017 -> ver 1.0

#include "RTClib.h"

// #ifdef __AVR__
//  #include <avr/pgmspace.h>
// #elif defined(ESP8266)
//  #include <pgmspace.h>
// #elif defined(ARDUINO_ARCH_SAMD)
// // nothing special needed
// #elif defined(ARDUINO_SAM_DUE)
//  #define PROGMEM
//  #define pgm_read_byte(addr) (*(const uint8_t *)(addr))
//  #define Wire Wire1
// #endif

// fix for raspberry
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))

static uint8_t read_i2c_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, 1);
  return Wire.read();
}

static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

////////////////////////////////////////////////////////////////////////////////
// utility code, some of this could be exposed in the DateTime API if needed

// const uint8_t daysInMonth [] PROGMEM = { 31,28,31,30,31,30,31,31,30,31,30,31 };
// Fix for raspberry pi
const uint8_t daysInMonth [] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

// number of days since 2000/01/01, valid for 2001..2099
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
        y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
        days += pgm_read_byte(daysInMonth + i - 1);
    if (m > 2 && y % 4 == 0)
        ++days;
    return days + 365 * y + (y + 3) / 4 - 1;
}

static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
    return ((days * 24L + h) * 60 + m) * 60 + s;
}

////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second

DateTime::DateTime (uint32_t t) {
   t -= SECONDS_FROM_1970_TO_2000;    // bring to 2000 timestamp from 1970

   ss = t % 60;
   t /= 60;
   mm = t % 60;
   t /= 60;
   hh = t % 24;
   uint16_t days = t / 24;
   uint8_t leap;
   for (yOff = 0; ; ++yOff) {
     leap = yOff % 4 == 0;
     if (days < 365 + leap)
         break;
     days -= 365 + leap;
   }
   for (m = 1; ; ++m) {
     uint8_t daysPerMonth = pgm_read_byte(daysInMonth + m - 1);
     if (leap && m == 2)
         ++daysPerMonth;
     if (days < daysPerMonth)
         break;
     days -= daysPerMonth;
   }
   d = days + 1;
}

DateTime::DateTime (uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    if (year >= 2000)
        year -= 2000;
    yOff = year;
    m = month;
    d = day;
    hh = hour;
    mm = min;
    ss = sec;
}

DateTime::DateTime (const DateTime& copy):
  yOff(copy.yOff),
  m(copy.m),
  d(copy.d),
  hh(copy.hh),
  mm(copy.mm),
  ss(copy.ss)
{}

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}

// A convenient constructor for using "the compiler's time":
//   DateTime now (__DATE__, __TIME__);
// NOTE: using F() would further reduce the RAM footprint, see below.
DateTime::DateTime (const char* date, const char* time) {
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    yOff = conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (date[0]) {
        case 'J': m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7; break;
        case 'F': m = 2; break;
        case 'A': m = date[2] == 'r' ? 4 : 8; break;
        case 'M': m = date[2] == 'r' ? 3 : 5; break;
        case 'S': m = 9; break;
        case 'O': m = 10; break;
        case 'N': m = 11; break;
        case 'D': m = 12; break;
    }
    d = conv2d(date + 4);
    hh = conv2d(time);
    mm = conv2d(time + 3);
    ss = conv2d(time + 6);
}

// A convenient constructor for using "the compiler's time":
// This version will save RAM by using PROGMEM to store it by using the F macro.
//   DateTime now (F(__DATE__), F(__TIME__));
// DateTime::DateTime (const __FlashStringHelper* date, const __FlashStringHelper* time) {
//     // sample input: date = "Dec 26 2009", time = "12:34:56"
//     char buff[11];
//     memcpy_P(buff, date, 11);
//     yOff = conv2d(buff + 9);
//     // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
//     switch (buff[0]) {
//         case 'J': m = buff[1] == 'a' ? 1 : m = buff[2] == 'n' ? 6 : 7; break;
//         case 'F': m = 2; break;
//         case 'A': m = buff[2] == 'r' ? 4 : 8; break;
//         case 'M': m = buff[2] == 'r' ? 3 : 5; break;
//         case 'S': m = 9; break;
//         case 'O': m = 10; break;
//         case 'N': m = 11; break;
//         case 'D': m = 12; break;
//     }
//     d = conv2d(buff + 4);
//     memcpy_P(buff, time, 8);
//     hh = conv2d(buff);
//     mm = conv2d(buff + 3);
//     ss = conv2d(buff + 6);
// }

uint8_t DateTime::dayOfTheWeek() const {
    uint16_t day = date2days(yOff, m, d);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

uint32_t DateTime::unixtime(void) const {
  uint32_t t;
  uint16_t days = date2days(yOff, m, d);
  t = time2long(days, hh, mm, ss);
  t += SECONDS_FROM_1970_TO_2000;  // seconds from 1970 to 2000

  return t;
}

long DateTime::secondstime(void) const {
  long t;
  uint16_t days = date2days(yOff, m, d);
  t = time2long(days, hh, mm, ss);
  return t;
}

DateTime DateTime::operator+(const TimeSpan& span) {
  return DateTime(unixtime()+span.totalseconds());
}

DateTime DateTime::operator-(const TimeSpan& span) {
  return DateTime(unixtime()-span.totalseconds());
}

TimeSpan DateTime::operator-(const DateTime& right) {
  return TimeSpan(unixtime()-right.unixtime());
}

////////////////////////////////////////////////////////////////////////////////
// TimeSpan implementation

TimeSpan::TimeSpan (int32_t seconds):
  _seconds(seconds)
{}

TimeSpan::TimeSpan (int16_t days, int8_t hours, int8_t minutes, int8_t seconds):
  _seconds((int32_t)days*86400L + (int32_t)hours*3600 + (int32_t)minutes*60 + seconds)
{}

TimeSpan::TimeSpan (const TimeSpan& copy):
  _seconds(copy._seconds)
{}

TimeSpan TimeSpan::operator+(const TimeSpan& right) {
  return TimeSpan(_seconds+right._seconds);
}

TimeSpan TimeSpan::operator-(const TimeSpan& right) {
  return TimeSpan(_seconds-right._seconds);
}

////////////////////////////////////////////////////////////////////////////////
// RTC_DS3231 implementation

bool RTC_DS3231::begin() {
   #if defined(ESP8266)
   Wire.begin(ESP8266_SDA, ESP8266_SCL);
   #else
   Wire.begin();
   #endif
   return true;
}

bool RTC_DS3231::lostPower(void) {
  return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG) >> 7);
}

void RTC_DS3231::adjust(const DateTime& dt) {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(0); // start at location 0
  Wire.write(bin2bcd(dt.second()));
  Wire.write(bin2bcd(dt.minute()));
  Wire.write(bin2bcd(dt.hour()));
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(dt.day()));
  Wire.write(bin2bcd(dt.month()));
  Wire.write(bin2bcd(dt.year() - 2000));
  Wire.endTransmission();

  uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  statreg &= ~0x80; // flip OSF bit
  write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg);
}

DateTime RTC_DS3231::now() {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 7);
  uint8_t ss = bcd2bin(Wire.read() & 0x7F);
  uint8_t mm = bcd2bin(Wire.read());
  uint8_t hh = bcd2bin(Wire.read());
  Wire.read();
  uint8_t d = bcd2bin(Wire.read());
  uint8_t m = bcd2bin(Wire.read());
  uint16_t y = bcd2bin(Wire.read()) + 2000;

  return DateTime (y, m, d, hh, mm, ss);
}

Ds3231SqwPinMode RTC_DS3231::readSqwPinMode() {
  int mode;

  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(DS3231_CONTROL);
  Wire.endTransmission();

  Wire.requestFrom((uint8_t)DS3231_ADDRESS, (uint8_t)1);
  mode = Wire.read();
  mode &= 0x93;
  return static_cast<Ds3231SqwPinMode>(mode);
}

void RTC_DS3231::writeSqwPinMode(Ds3231SqwPinMode mode) {
  uint8_t ctrl;
  ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);

  ctrl &= ~0x04; // turn off INTCON
  ctrl &= ~0x18; // set freq bits to 0

  if (mode == DS3231_OFF) {
    ctrl |= 0x04; // turn on INTCN
  } else {
    ctrl |= mode;
  }
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
}

uint8_t RTC_DS3231::readRegControl() {
   return read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
}

bool RTC_DS3231::switchClock(bool state) {
   uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
   if(state)
      ctrl |= 0x04;  // turn on INTCN
   else
      ctrl &= ~0x04; // turn off INTCN
   write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
   return true;
}

bool RTC_DS3231::isClockOn() {
   return  (read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL) & 0x04) ? true : false;
}

bool RTC_DS3231::setAlarm(uint8_t alarm) {
   clearAlarm(alarm);
   uint8_t ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);
   ctrl |= 0x04;  // turn on INTCN
   ctrl |= alarm;  // set bit alarm
   write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);
   return true;
}

bool RTC_DS3231::setAlarmRepeat(uint8_t alarm) {
   setAlarm(alarm);
   Wire.beginTransmission(DS3231_ADDRESS);
   Wire.write(alarm_addr[alarm-1]);
   if(alarm==DS3231_ALARM_1) {
      Wire.write(0x80);                    // bit A*M1 = 1
   }
   Wire.write(0x80);                    // bit A*M1 = 1
   Wire.write(0x80);                    // bit A*M1 = 1
   Wire.write(0x80);                    // bit A*M1 = 1
   Wire.endTransmission();
   return true;
}

bool RTC_DS3231::setAlarm(uint8_t alarm, alarm_t tm, uint8_t mode) {

   setAlarm(alarm);

   Wire.beginTransmission(DS3231_ADDRESS);
   Wire.write(alarm_addr[alarm-1]);

   if(alarm==DS3231_ALARM_1) {
      if(mode>0)
         Wire.write(bin2bcd(tm.ss) & ~0x80 );    // bit A*M1 = 0
      else
         Wire.write(0x80);                    // bit A*M1 = 1
   }
   if(mode>1)
      Wire.write(bin2bcd(tm.mm) & ~0x80 );       // bit A*M2 = 0
   else
      Wire.write(0x80);                       // bit A*M2 = 0

   if(mode>2)
      Wire.write(bin2bcd(tm.hh) & ~0x80 );       // bit A*M3 = 0
   else
      Wire.write(0x80);                       // bit A*M3 = 1

   if(mode>3)
      Wire.write(bin2bcd(tm.d) & ~0xC0 );     // bit A*M4 = 0, bit DY/DT = 0
   else
      Wire.write(0x80);                       // bit A*M4 = 1

   Wire.endTransmission();

   return true;
   // This code below to check Time was set right or not
   // alarm_t tmr = readAlarmTime(alarm);
   // return (tmr.d==tm.d && tmr.hh==tm.hh && tmr.mm==tm.mm && tmr.ss==tm.ss) ? true : false;
}

bool RTC_DS3231::setAlarmHour(uint8_t alarm, uint8_t hh) {

   setAlarm(alarm);
   Wire.beginTransmission(DS3231_ADDRESS);
   Wire.write(alarm_addr[alarm-1]);
   if(alarm==DS3231_ALARM_1)
      Wire.write(0);                             // second = 0 and A*M1 = 0
   Wire.write(0);                                // minute = 0 and A*M2 = 0
   Wire.write(bin2bcd(hh) & ~0x80);              // hour = hh and A*M3 = 0
   Wire.write(0x80);                             // A*M4 = 1;
   Wire.endTransmission();

   return true;
   // This code below to check Time was set right or not
   // return (hh==readAlarmTime(alarm).hh) ? true:false;
}

alarm_t RTC_DS3231::readAlarmTime(uint8_t alarm) {

   alarm_t tm = {0};

   Wire.beginTransmission(DS3231_ADDRESS);
   Wire.write(alarm_addr[alarm-1]);
   Wire.endTransmission();

   if(alarm==DS3231_ALARM_1) {
      Wire.requestFrom(DS3231_ADDRESS, 4);
      tm.ss = bcd2bin(Wire.read());
   }
   else
      Wire.requestFrom(DS3231_ADDRESS, 3);

   tm.mm  = bcd2bin(Wire.read());
   tm.hh = bcd2bin(Wire.read());
   tm.d = bcd2bin(Wire.read());

   if(tm.ss>=80) tm.ss=0;
   if(tm.mm>=80) tm.mm=0;
   if(tm.hh>=80) tm.hh=0;
   if(tm.d>=80) tm.d=0;

   return tm;
}

bool RTC_DS3231::getDateTimeString(char *s) {
   DateTime tm = now();
   if(tm.unixtime()==RCT_WRONG)
      return false;
   snprintf (s, 22, "%d/%d/%dT%d:%s%d:%s%d", tm.year(), tm.month(), tm.day(), tm.hour(), ((tm.minute() > 9) ? "": "0"), tm.minute(), ((tm.second() > 9) ? "": "0"), tm.second());
   return true;
}

bool RTC_DS3231::getDateString(char *s) {
   DateTime tm = now();
   if(tm.unixtime()==RCT_WRONG)
      return false;
   snprintf (s, 10, "%d/%d/%d", tm.year(), tm.month(), tm.day());
   return true;
}

bool RTC_DS3231::getTimeString(char *s) {
   DateTime tm = now();
   if(tm.unixtime()==RCT_WRONG)
      return false;
   snprintf (s, 10, "%d:%s%d:%s%d", tm.hour(), ((tm.minute() > 9) ? "": "0"), tm.minute(), ((tm.second() > 9) ? "": "0"), tm.second());
   return true;
}

uint8_t RTC_DS3231::readRegStatus() {
   return read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
}

uint8_t RTC_DS3231::readRegister(uint8_t reg) {
   return read_i2c_register(DS3231_ADDRESS, reg);
}

bool RTC_DS3231::isAlarmSet(uint8_t alarm) {
   return  (read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL) & alarm) ? true : false;
}

bool RTC_DS3231::isAlarmRinging(uint8_t alarm) {
   return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG) & alarm) ? true : false;
}

bool RTC_DS3231::clearAlarm(uint8_t alarm) {
   uint8_t regStatus = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
   if(alarm==0)
      regStatus &= ~0x03;
   else
      regStatus &= ~alarm;
   write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, regStatus);
   return true;
}
