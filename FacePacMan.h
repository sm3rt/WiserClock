/*
 *********************************************************************************************************
 * Adapated from WiseClock3
 *
 * Apr/10 by FlorinC (http://timewitharduino.blogspot.com/)
 *   Copyrighted and distributed under the terms of the Berkeley license
 *   (copy freely, but include this notice of original authors.)
 *
 *********************************************************************************************************
 */


#include "ClockFace.h"
#include "HT1632.h"

struct PacManFace: 
public ClockFace {
//  long dotMillis;
//  bool dotsVisible;

  virtual void updateDisplay() {
    // (fc, June 10/2011) Pacman mode;
    char	timeBuffer[12];		
    //byte color = getColor();

    //displayTimeWithSeconds(5);
    // build the string containing the formatted time;
    int hpos = 5;
    timeBuffer[0]	=	(hours < 10) ? ' ' : ('0' + hours/10);
    timeBuffer[1]	=	'0' + hours % 10;
    timeBuffer[2]	=	':';
      // timeBuffer[2]	=	isAlarmEnabled? ':' : '.';
    timeBuffer[3]	=	'0' + minutes / 10;
    timeBuffer[4]	=	'0' + minutes % 10;
    timeBuffer[5]	=	':';
      // timeBuffer[5]	=	isAlarmEnabled? ':' : '.';
    timeBuffer[6]	=	'0' + seconds / 10;
    timeBuffer[7]	=	'0' + seconds % 10;

    byte digits = 8;
    if (hpos > 15)			// if hpos > 15 then no seconds;
    {	
      hpos = hpos - 16;
      digits = 5;
    }	
    // display time with or without seconds on hpos;
    for (int i=0; i<digits; i++)
    {
      ht1632_putTinyChar(i*4-1, hpos, timeBuffer[i], GREEN);
    }


    if (seconds >55)
    {
      // display alternatively the pacman bitmaps;
      for (int i=32; i>-14; i--)
      {
        ht1632_putBitmap(i, 1, abs(i)%7);
        delay (100);
      }
    }
  }
};





