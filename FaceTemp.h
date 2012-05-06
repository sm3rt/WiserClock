// facetemp.h

#ifndef _APP_TEMP_H_
#define _APP_TEMP_H_

#include <Arduino.h>
#include "ClockFace.h"
#include "DallasTemperature.h"


class TempFace: public ClockFace
{
private:
  unsigned int lastminute;
 
 public:
  virtual void init();
  virtual void updateDisplay();

private:

  
};

#endif  // _APP_TEMP_H_


