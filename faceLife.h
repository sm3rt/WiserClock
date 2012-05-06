// AppLife.h

#ifndef _APP_LIFE_H_
#define _APP_LIFE_H_

#include <Arduino.h>
#include "ClockFace.h"

class LifeFace: public ClockFace
{

public:
  virtual void init();
  virtual void updateDisplay();
};




#endif  // _APP_LIFE_H_


