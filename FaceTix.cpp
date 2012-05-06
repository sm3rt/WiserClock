// AppTix.cpp
// show time HH:MM as in TIX Clock

//*********************************************************************************************************
//*	Edit History, started Oct, 2011
//*	please put your initials and comments here anytime you make changes
//*********************************************************************************************************
//*	Oct  15,	2011	(rp) first entry;
//* QVS wiserclock format
//*********************************************************************************************************


#include "FaceTix.h"
#include "HT1632.h"

// there are 27 squares in 4 groups, 3 + 9 + 6 + 9;
byte tixColor[27];											// contains color for each square;
byte  tixXPos[9] 	= {
  1,  5,8,11,  15,18,  22,25,28};		// contains horizontal position for each column;
byte  tixYPos[3] 	={ 
  1,6,11};								// contains vertical postion for each row;
byte  tixText[13]	= {
  3,6,7,8,9,12,13,14,18,20,22,24,26};	// contains square numbers for TIX start up text;
//*********************************************************************************************************
void TixFace::init()
{
  byte i, j, k;
  tixMillis = 0;
  tixSpeed = 10000; // 10 Sec
  for (i = 0; i < 13; i++)
  {
    for (j = 0; j < 3; j++)
    {
      for (k = 0; k < 4; k++)
        ht1632_plot(tixXPos[tixText[i] / 3] + j, tixYPos[tixText[i] % 3] + k, RED); 	// display TIX text;
    }
  }
  delay(2000);
}

//*********************************************************************************************************
void TixFace::updateDisplay()
{

  byte i, j, k;

  long now = millis();
  if (now-tixMillis > tixSpeed)
  {	
    tixMillis = now;

    for (i = 0; i < 27; i++)
      tixColor[i] = BLACK;						// set all 27 squares to black;

    setTixColor(hours / 10, 0, 3, GREEN);			// set squares for first  digit to correct color;
    setTixColor(hours % 10, 3, 12, ORANGE);			// set squares for second digit to correct color;
    setTixColor(minutes / 10, 12, 18, GREEN);		// set squares for third  digit to correct color;		
    setTixColor(minutes % 10, 18, 27, ORANGE);		// set squares for fourth digit to correct color;

    for (i = 0; i < 27; i++)
    {
      for (j = 0; j < 3; j++)
      {
        for (k = 0; k < 4; k++)
          ht1632_plot(tixXPos[i / 3] + j, tixYPos[i % 3] + k, tixColor[i]); 	// display all 27 squares
      }
    }
  }
}


//*********************************************************************************************************
void TixFace::setTixColor(byte number, byte first, byte last, byte color)
{
  byte j;

  while (number > 0)
  {
    j = random(first, last);
    if (tixColor[j] == BLACK)
    {
      tixColor[j] = color;
      number--;
    }
  }
}








