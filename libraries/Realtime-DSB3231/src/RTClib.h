// Author : Nguyen Hai Duong
// Jan 2017
// Part of library by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!

// Version 1.0

#ifndef _RTCLIB_H_
#define _RTCLIB_H_

// #include <Arduino.h>
// #include <Wire.h>
// Fix for raspberry
#include <stdint.h>
#include "Wire/Wire.h"

class TimeSpan;

#define DS3231_ADDRESS  0x68
#define DS3231_CONTROL  0x0E
#define DS3231_STATUSREG 0x0F
const uint8_t alarm_addr[2] = {0x07,0x0B};
#define DS3231_ALARM_1 1
#define DS3231_ALARM_2 2

#define SECONDS_PER_DAY 86400L
#define SECONDS_FROM_1970_TO_2000 946684800
#define RCT_WRONG 947979985            // it will be used to indicate that reading from RTC has a problem

#if defined(ESP8266)
#define ESP8266_SDA 2         // esp8266 ver7
#define ESP8266_SCL 14        // esp8266 ver7
#endif

// This is define for public domain. The other file should just use it
#define ALARM_1 DS3231_ALARM_1
#define ALARM_2 DS3231_ALARM_2
#define RTC RTC_DS3231
// define for use with alarm mode.
#define EACH_SS_OR_MM 0             // repeat each second with alarm 1, repeat each minute with alarm 2
#define MATCH_SS 1                  // match second with alarm 1
#define MATCH_MMSS_OR_MM 2          // match minute and second with alarm 1, match minute with alarm 2
#define MATCH_HHMMSS_OR_HHMM 3      // match hour + minute + second with alarm 1, match hour + minute with alarm 2
#define MATCH_DDHHSS_OR_DDHHMM 4    // match date + hour + minute + second with alarm1, match date + hour + minute with alarm 2
//  end define

// alarm just support max matched dayofmonth/hours/minutes/second
typedef struct Alarm {
   uint8_t d, hh, mm, ss;
} alarm_t;

// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
class DateTime {
public:
   DateTime (uint32_t t =0);
   DateTime (uint16_t year, uint8_t month, uint8_t day,
            uint8_t hour =0, uint8_t min =0, uint8_t sec =0);
   DateTime (const DateTime& copy);
   DateTime (const char* date, const char* time);
   // DateTime (const __FlashStringHelper* date, const __FlashStringHelper* time);
   uint16_t year() const       { return 2000 + yOff; }
   uint8_t month() const       { return m; }
   uint8_t day() const         { return d; }
   uint8_t hour() const        { return hh; }
   uint8_t minute() const      { return mm; }
   uint8_t second() const      { return ss; }
   uint8_t dayOfTheWeek() const;

   // 32-bit times as seconds since 1/1/2000
   long secondstime() const;
   // 32-bit times as seconds since 1/1/1970
   uint32_t unixtime(void) const;

   DateTime operator+(const TimeSpan& span);
   DateTime operator-(const TimeSpan& span);
   TimeSpan operator-(const DateTime& right);

protected:
   uint8_t yOff, m, d, hh, mm, ss;
};

// Timespan which can represent changes in time with seconds accuracy.
class TimeSpan {
public:
   TimeSpan (int32_t seconds = 0);
   TimeSpan (int16_t days, int8_t hours, int8_t minutes, int8_t seconds);
   TimeSpan (const TimeSpan& copy);
   int16_t days() const         { return _seconds / 86400L; }
   int8_t  hours() const        { return _seconds / 3600 % 24; }
   int8_t  minutes() const      { return _seconds / 60 % 60; }
   int8_t  seconds() const      { return _seconds % 60; }
   int32_t totalseconds() const { return _seconds; }

   TimeSpan operator+(const TimeSpan& right);
   TimeSpan operator-(const TimeSpan& right);

protected:
   int32_t _seconds;
};

// RTC based on the DS3231 chip connected via I2C and the Wire library
enum Ds3231SqwPinMode { DS3231_OFF = 0x01, DS3231_SquareWave1Hz = 0x00, DS3231_SquareWave1kHz = 0x08, DS3231_SquareWave4kHz = 0x10, DS3231_SquareWave8kHz = 0x18 };

class RTC_DS3231 {

public:
   bool begin();                      // setup I2C
   static void adjust(const DateTime& dt);   // set time to RTC
   bool lostPower(void);                     // check time of RTC was set or alarmt.
   static DateTime now();                    // get time from RTC
   bool getDateTimeString(char *s);      // get time + date and convert it to string. Time format ISO 8601 : yyyy:mm:ddThh:mm:ss
   bool getDateString(char *s);      // get date and convert it to string. Date format ISO 8601 : yyyy:mm:dd
   bool getTimeString(char *s);          // get time and convert it to string. Time format ISO 8601 hh:mm:ss
   static Ds3231SqwPinMode readSqwPinMode(); // read square wave in SQW pin
   static void writeSqwPinMode(Ds3231SqwPinMode mode);   // write mode to SQW pin (there are 5 modes)
   bool switchClock(bool state);          // switch on/off clock by set INTCN. Set clock to use alarms
   bool isClockOn();                      // check clock has turned on or off by check bit INTCN = 0 or 1
   // Turn on alarm. set bit INTCN = 1 and set bit alarm = 1
   bool setAlarm(uint8_t alarm);         // there are 2 alarms. uint8_t alarm = 1 or 2 (alarm 1 or 2)
   // Table : alarm and mode, ss (second), mm (minute), hh (hour), date (day of month)
   //             Alarm 1            Alarm 2             mode
   //          repeat second     repeat a minute         0
   // match :     ss                                     1
   // match :    mm,ss                 mm                2
   // match :   hh,nn,ss             hh,mm               3
   // match : date,hh,mm,ss         date,hh,mm           4
   bool setAlarmRepeat(uint8_t alarm); // mode = 0
   bool setAlarm(uint8_t alarm, alarm_t tm, uint8_t mode = MATCH_HHMMSS_OR_HHMM); // mode = 1 -> 4, default = 3
   bool setAlarmHour(uint8_t alarm, uint8_t hour);  // set alarm on if time equal hour of day (24 hour). ex setAlarmHour(DS3231_ALARM_2, 20). Alarm 2 will on at 20:00:00 (hh:mm:ss)
   bool isAlarmSet(uint8_t alarm);                 // check alarm has set or not.  uint8_t alarm = 1 or 2
   bool clearAlarm(uint8_t alarm=0);               // clear flag of alarm. default = 0 means clear all flags.
   bool isAlarmRinging(uint8_t alarm);             // check flag has turned on or not
   alarm_t readAlarmTime(uint8_t alarm);  // read time has set for alarm

private:
   uint8_t readRegControl();              // read control register
   uint8_t readRegStatus();               // read status register
   uint8_t readRegister(uint8_t reg);     // read reg register

};

#endif // _RTCLIB_H_
