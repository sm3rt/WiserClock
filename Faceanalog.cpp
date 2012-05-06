/*********************************************************************************************************
 * Parts...  Apr/10 by FlorinC (http://timewitharduino.blogspot.com/)
 *   Copyrighted and distributed under the terms of the Berkeley license
 *   (copy freely, but include this notice of original authors.)
 *
 *********************************************************************************************************
 */

#include "ClockFace.h"
#include "HT1632.h"
#include "faceanalog.h"

void AnalogFace::init() {
  
}	

void AnalogFace::updateDisplay() {
  //*********************************************************************************************************
// Display time as analog clock;
//

// LED Positions for analog clock;
byte 	PROGMEM clk[24] = {      15,2, 		 18,3, 		 20,5, 		 21,8, 		  20,11, 		18,13, 		  15,14, 		12,13, 		  10,11,  		9,8, 	   10,5, 	   12,3};
byte 	PROGMEM hrs[52] = {14,4, 15,3, 16,4, 17,4, 18,5, 19,6, 19,7, 20,8, 19, 9, 19,10, 18,11, 17,12, 16,12, 15,13, 14,12, 13,12, 12,11, 11,10, 11,9, 10,8, 11,7, 11,6, 12,5, 13,4, 14,4, 15,3};
byte 	PROGMEM mns[52] = {13,1, 15,1, 17,1, 19,2, 20,3, 21,4, 22,6, 22,8, 22,10, 21,12, 20,13, 19,14, 17,15, 15,15, 13,15, 11,14, 10,13,  9,12,  8,10, 8,8,  8,6,  9,4, 10,3, 11,2, 13,1, 15,1};

//void WiseClock3::showAnaClock(int o, int u)
//{
	// QVS removed these
	#define o  0
	#define u  0
	
	byte x, hh, mm, ss;

	for (x = 0; x < 24; x = x + 2)
		ht1632_plot(clk[x] + o, clk[x + 1], GREEN);			// show clock;
	
	hh = (hours + u + 24) % 24;
	mm = minutes;
	ss = seconds;
	
	if (hh > 11)
		hh = hh - 12;
	hh = hh << 2;

	if (mm > 47)
		hh = hh + 4;
	else
		if (mm > 12)
			hh = hh + 2;
			
	ht1632_plot(hrs[hh] + o, hrs[hh + 1], BLACK);			// clear previous hour;
	ht1632_plot(hrs[hh + 2] + o, hrs[hh + 3], ORANGE);		// hours;

	if (ss & 1)
		ht1632_plot(15 + o,8, RED);							// alternate center dot red / orange;
	else	
		ht1632_plot(15 + o,8, ORANGE);
		
	ss = (((ss + 1) << 1) / 5) << 1;
	mm = (((mm + 1) << 1) / 5) << 1;

	ht1632_plot(mns[mm] + o, mns[mm + 1], BLACK);			// clear previous minute;
	ht1632_plot(mns[mm + 2] + o, mns[mm + 3], RED);			// minutes;

	if (o == 0)
	{
		if (ss != (mm + 2))
			ht1632_plot(mns[ss], mns[ss + 1], BLACK);		// clear previous second;

		if (ss != mm)
			ht1632_plot(mns[ss + 2], mns[ss + 3], GREEN);	// seconds;
	}		
}





