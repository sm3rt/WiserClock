// FACETemp.cpp
// show time HH:MM as in TIX Clock

//*********************************************************************************************************
//* Quentin Smart April 2012
//*********************************************************************************************************


#include "FaceTemp.h"
#include "HT1632.h"


#define ONE_WIRE_BUS 7

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire ds(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&ds);
byte tempMin[32] = {
  0};
byte position = 0;	
DeviceAddress deviceAddress;

//*********************************************************************************************************
void TempFace::init()
{
  lastminute = 61;
  sensors.begin();
  if (!(sensors.getAddress(deviceAddress, 0))) {
 // error
  }		
}

//*********************************************************************************************************
void TempFace::updateDisplay()
{
  // Get the Temperature every min
  if (minutes != lastminute) {
    lastminute = minutes;
    sensors.requestTemperatures();	  
    float temperature = sensors.getTempC(deviceAddress);	  
    int t = temperature;
    // display temp
    ht1632_putSmallChar( 5, -1,('0' + t/10) , RED);
    ht1632_putSmallChar(10, -1,('0' + t % 10) , RED);
  }
}








