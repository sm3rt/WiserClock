// AppTix.h

#ifndef _APP_TIX_H_
#define _APP_TIX_H_

#include <Arduino.h>
#include "ClockFace.h"


class TixFace: public ClockFace
{
private:
  boolean startupAppTix;
  unsigned long tixMillis;
  unsigned int tixSpeed;
 
 public:
  virtual void init();
  virtual void updateDisplay();

private:
  void setTixColor(byte number, byte first, byte last, byte color);
  
};

#endif  // _APP_TIX_H_
