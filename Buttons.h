// Buttons.h

#ifndef _BUTTONS_H_
#define _BUTTONS_H_


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <CapSense.h>

enum ButtonType {
  BUTTON_MENU, BUTTON_SET, BUTTON_PLUS
};

extern void (*buttonCallback)(ButtonType);

void checkButtons();

#endif  // _BUTTONS_H_


