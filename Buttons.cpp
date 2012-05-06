// Buttons.cpp


#include "Buttons.h"

#define BOUNCE_TIME_BUTTON		600	// bounce time in ms for the menu button;

#define CapThreshold 100

CapSense   but_1 = CapSense(8,9);        // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
CapSense   but_2 = CapSense(8,10);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
CapSense   but_3 = CapSense(8,11);        // 10M resistor between pins 4 & 8, pin 8 is sensor pin, add a wire and or foil



void (*buttonCallback)(ButtonType) = NULL;

//*********************************************************************************************************
void checkButtons()
{
	// last time the respective button was pressed; used for debouncing;
static unsigned long timeBtnMenu;
static unsigned long timeBtnSet;
static unsigned long timeBtnPlus;

  if (but_1.capSense(30)  > CapThreshold) 
  {
    if (abs(millis() - timeBtnMenu) < BOUNCE_TIME_BUTTON)  return;

    if(buttonCallback != NULL)
      buttonCallback(BUTTON_MENU);

    timeBtnMenu	=	millis();
  }	

  if (but_2.capSense(30) > CapThreshold) 
  {
    // debouncing;
    if (abs(millis() - timeBtnSet) < BOUNCE_TIME_BUTTON)  return;

    if(buttonCallback != NULL)
      buttonCallback(BUTTON_SET);

    timeBtnSet	=	millis();
  }

  if (but_3.capSense(30) > CapThreshold) 
  {

    // debouncing;
    if (abs(millis() - timeBtnPlus) < BOUNCE_TIME_BUTTON)  return;
    if(buttonCallback != NULL)
      buttonCallback(BUTTON_PLUS);
    timeBtnPlus	=	millis();
  }

}	
//*********************************************************************************************************





