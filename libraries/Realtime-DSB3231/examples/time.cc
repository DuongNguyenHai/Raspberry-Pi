// g++ -Wall -std=c++11 time.cc ../src/RTClib.cpp ../src/Wire/Wire.cpp -o time

#include <stdio.h>
#include "../src/RTClib.h"

RTC_DS3231 rtc;

int main(int argc, char const *argv[])
{
   char tm[22];

   if (! rtc.begin()) {
      printf("Couldn't find RTC\n");
      while (1);
   }

   if (rtc.lostPower()) {     // check RTC time has set or not.
      printf("RTC lost power, lets set the time!");
      // following line sets the RTC to the date & time.
      //  adjust(DateTime(year,month,day,hour,minute,second))
      rtc.adjust(DateTime(2017, 01, 1, 00, 00, 00));
   }


   while(1) {
      rtc.getDateTimeString(tm);
      printf("Date time: %s\n", tm);
      usleep(1000000);
   }

   return 0;
}