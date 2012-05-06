#include "ClockFace.h"
#include "HT1632.h"
#include "facesimple.h"
#include "DS1307.h"

#define rtc RTC

void SimpleFace::init() {
  isTimeSetting = 0; 
  dotMillis = 0;
}	

void SimpleFace::updateDisplay() {
  byte hourColor = GREEN;
  byte minColor = GREEN;

  if (isTimeSetting) {
    if (((millis() / 250)  % 2) == 0) {
      if (isTimeSetting == 1)
        hourColor = BLACK;
      else
        minColor = BLACK;
    }
  }

  ht1632_putBigDigit(2, 2, (hours < 10) ? 10 : hours/10, 1, hourColor, 6);
  ht1632_putBigDigit(8, 2, hours % 10, 1, hourColor, 6);
  ht1632_putBigDigit(16, 2, minutes/10, 1, minColor, 6);
  ht1632_putBigDigit(22, 2, minutes%10, 1, minColor, 6);

  long now = millis();
  if(now-dotMillis > 800) {
    dotMillis = now;

    dotsVisible = !dotsVisible;
    byte color = dotsVisible ? BLACK : GREEN;

    ht1632_plot(15, 6, color);
    ht1632_plot(15, 9, color);
  }
}

// QVS Set time
void SimpleFace::setPressed() {
  if (!isTimeSetting) 
    isTimeSetting = 1;
  else
    isTimeSetting = isTimeSetting ^ 3;


}

void SimpleFace::plusPressed() {
  if (isTimeSetting == 1) {
    if (++hours > 23) 
      hours = 0;    

    rtc.stop();
    rtc.set(DS1307_HR,hours);
      rtc.start();
  }		
  else
    if (isTimeSetting == 2){
      if (++minutes > 59) 
        minutes = 0;      
 
      rtc.stop();
      rtc.set(DS1307_MIN,minutes);
        rtc.start();
    }		
}







