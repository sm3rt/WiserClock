#include <OneWire.h>

#include "ClockController.h"
#include "ClockFace.h"
//#include "Sound.h"
//#include "Settings.h"
#include "HT1632.h"
#define rtc RTC




void ClockController::setup() {
  changeFace(faceManager.currentFace());

//  beepEnabled = EEPROM.read(SETTINGS_BEEP_ENABLED);
//  beepTriggerEnabled = false;
	tick();

 // Serial.begin(9600);
}

void ClockController::tick() {
  int	time[7];

  rtc.get(time, true);

  seconds  = time[0];
  minutes  = time[1];
  hours    = time[2];
  //int dow     = time[3];
  day     = time[4];
  month   = time[5];
  year    = time[6];


 
 // Serial.println(temperature);

 // temperature = rtc.getTemperature();

  if(currentFace != NULL) {
    if(seconds != currentFace->seconds ||
       minutes != currentFace->minutes ||
       hours   != currentFace->hours)
      currentFace->setTime(hours, minutes, seconds);

    if(day   != currentFace->day ||
       month != currentFace->month ||
       year  != currentFace->year)
      currentFace->setDate(day, month, year);

    if(temperature != currentFace->temperature)
      currentFace->setTemperature(temperature);

    currentFace->lastUpdate = millis();
    currentFace->updateDisplay();
  }

//  checkForBeep();
 // checkSerial();
}

void ClockController::buttonClicked(ButtonType button) {
  if(button == BUTTON_MENU) {
    changeFace(faceManager.nextFace());
  }
  else if (button == BUTTON_SET) {
	  currentFace->setPressed();
  }
  else if (button == BUTTON_PLUS) {
		currentFace->plusPressed();
  }
  
}

void ClockController::changeFace(ClockFace* newFace) {
  currentFace = newFace;
// QVS Changed sequence so init can draw on screen and not get erased.
  ht1632_clear();
  currentFace->init();
  }



/*
void ClockController::checkForBeep() {
  if(!beepEnabled) return;

  if(seconds > 30) {
    beepTriggerEnabled = true;
  } else if(beepTriggerEnabled) {
    if(minutes == 0)
      soundChimeLong();
    else if(minutes == 30)
      soundChimeShort();

    if(minutes == 0 || minutes == 30)
      beepTriggerEnabled = false;
  }
}
*/

void ClockController::checkSerial() {
  byte incoming;
  while(Serial.available()) {
    incoming = Serial.read();
    if(incoming == '\n') {
      //Serial.println("Got line: " + buffer);
      dispatchSerial(buffer);
      buffer = "";
    } else
      buffer += (char)incoming;
  }
}

bool ClockController::handleSerialEvent(const String& line) {
  bool handled = false;
  if(line.startsWith("time ")) {
    String timeString = line.substring(5);
    int idx = timeString.indexOf(':');
    if(idx != -1) {
      int hours = timeString.substring(0,2).toInt();
      int mins  = timeString.substring(3,5).toInt();
      Serial.print("Setting time to: ");
      Serial.print(hours);
      Serial.print(':');
      Serial.println(mins);

      rtc.stop();
      rtc.set(DS1307_SEC, 0);
      rtc.set(DS1307_MIN, mins);
      rtc.set(DS1307_HR, hours);
      rtc.start();
    } else {
      Serial.println("Couldn't parse time: " + timeString);
    }

    handled = true;
  } else if(line.startsWith("date ")) {
    String dateString = line.substring(5);
    int idx = dateString.indexOf('/');
    if(idx != -1) {
      int day = dateString.substring(0,2).toInt();
      int month  = dateString.substring(3,5).toInt();
      int year  = dateString.substring(6,10).toInt();
      Serial.print("Setting date to: ");
      Serial.print(day);
      Serial.print('/');
      Serial.print(month);
      Serial.print('/');
      Serial.println(year);

      rtc.stop();
      rtc.set(DS1307_DATE, day);
      rtc.set(DS1307_MTH, month);
      rtc.set(DS1307_YR, year);
      rtc.start();
    } else if(line.startsWith("hourBeep ")) {
      bool enabled = (line.substring(9,10).toInt()==1);
  //    EEPROM.write(SETTINGS_BEEP_ENABLED, enabled);
      beepEnabled = enabled;
      Serial.print(beepEnabled ? "Enabling" : "Disabling");
      Serial.println(" beep");
      handled = true;
    } else {
      Serial.println("Couldn't parse date: " + dateString);
    }

    handled = true;
  }

  if(!handled) {
    //Dispatch to child(s)
    if(currentFace != NULL)
      handled = currentFace->handleSerialEvent(line);
  }

  return handled;
}

void ClockController::dispatchSerial(const String& line) {
  bool handled = false;

  if(line == "beep") {
 //   beep();
    handled = true;
  } else if(line.equalsIgnoreCase("Open the pod bay doors, HAL")){
    Serial.println("Sorry. I can't let you do that, Dave.");
    handled = true;
  } else {
    //Dispatch
    handled = this->handleSerialEvent(line);
  }

  if(!handled)
    Serial.println("I'm not wise enough to understand: " + line);
}


