// AppLife.cpp

//*********************************************************************************************************
//*	Edit History, started June, 2011
//*	please put your initials and comments here anytime you make changes
//*********************************************************************************************************
//*	Jun  20,	2011	(rp) added random color to "case 3";
//* QVS moved to wiserclock format
//*********************************************************************************************************


#include "faceLife.h"
#include "HT1632.h"

int Generations = 0;
#define GEN 200

void LifeFace::init()
{
  // start a new game of life (from a newly generated patern);
  //  Serial.begin(9600);
  Generations = 0;
}


void LifeFace::updateDisplay()
{
  byte x,y;

  if (Generations-- == 0) {
    char timeBuffer[6];
    
    timeBuffer[0]	=	(hours < 10) ? ' ' : ('0' + hours/10);
    timeBuffer[1]	=	'0' + hours % 10;
    timeBuffer[2]	=	':';
    timeBuffer[3]	=	'0' + minutes / 10;
    timeBuffer[4]	=	'0' + minutes % 10;  
    timeBuffer[5]       =  0;
      
      
    //randomText[0]		=	random(32, 127);
    //randomText[1]		=	random(32, 127);
    //randomText[2]		=	random(32, 127);
    displayStaticLine(timeBuffer, 1, GREEN); 
    delay(1000);
    Generations = GEN;
  }

  snapshot_shadowram();


  for (x=0; x < X_MAX; x++)
  {
    byte xabove = (x == X_MAX-1) ? 0 : x+1;
    byte xbelow = (x == 0) ? X_MAX-1 : x-1;

    for (y=0; y < Y_MAX; y++)
    {
      byte neighbors = 0;
      byte yabove = (y == Y_MAX-1) ? 0 : y+1;
      byte ybelow = (y == 0) ? Y_MAX-1 : y-1;

      if (get_snapshotram(x, yabove) > 0) neighbors++;
      if (get_snapshotram(x, ybelow) > 0) neighbors++;

      if (get_snapshotram(xabove, y) > 0) neighbors++;
      if (get_snapshotram(xabove, yabove) > 0) neighbors++;
      if (get_snapshotram(xabove, ybelow) > 0) neighbors++;

      if (get_snapshotram(xbelow, y) > 0) neighbors++;
      if (get_snapshotram(xbelow, yabove) > 0) neighbors++;
      if (get_snapshotram(xbelow, ybelow) > 0) neighbors++;

      byte newval = get_snapshotram(x,y);

      switch (neighbors)
      {
        // Conditions picked up by default...
        //		case 0:
        //		case 1:
        //			newval	=	BLACK;	// death by loneliness
        //			break;

      case 2:
        if (newval != BLACK)
          newval = ORANGE;
        break;			// remains the same

      case 3:
 //       newval	=	random(GREEN, ORANGE + 1);
          if (newval == BLACK)
              newval = GREEN;
          else
              newval = RED;
        break;

      default:
        newval	=	BLACK;	// death by overcrowding
        break;
      }

      ht1632_plot(x,y, newval);

    }
  }
  snapshot_shadowram();
  delay(800);
}









