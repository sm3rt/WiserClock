#include "ds1307.h"
#include "ClockFace.h"
#include "Buttons.h"
#include "FaceManager.h"
#include "SerialHandler.h"

struct ClockController: public SerialHandler {
  //RTC rtc;
  int hours,minutes,seconds,day,month,year;
  float temperature;

  FaceManager faceManager;
  ClockFace* currentFace;

  bool beepEnabled;

  void setup();
  void tick();

  void buttonClicked(ButtonType button);

  void changeFace(ClockFace* newFace);
  
 
private:

  String buffer;
  void checkSerial();
  void dispatchSerial(const String& line);

  bool handleSerialEvent(const String& line);

};
