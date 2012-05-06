#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include <EEPROM.h>
#include <CapSense.h>
#include <OneWire.h>


#include "ds1307.h"
//#include "DS3231.h"
#include "HT1632.h"
//#include "SDuFAT.h"
//#include "Sound.h"
#include "Buttons.h"
#include "ClockController.h"

ClockController controller;

void buttonClicked(ButtonType button) {
  controller.buttonClicked(button);
}

void setup() {
//	setupSpeaker();
//	beep();
	ht1632_setup();

    randomSeed(analogRead(A0));

    controller.setup();

    //Connect button callbacks
    buttonCallback = &buttonClicked;
}

void loop() {
  controller.tick();
  checkButtons();
}
