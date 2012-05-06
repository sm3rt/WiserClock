#ifndef PONG_FACE_H
#define PONG_FACE_H

#include <Arduino.h>
#include "ClockFace.h"

class PongFace: public ClockFace
{
private:
  int ballpos_x;
  int ballvel_x;
  int ballpos_y;	// scaled by 256
  int ballvel_y;	// scaled by 256
  byte erase_x;  //holds ball old pos so we can erase it, set to blank area of screen initially.
  byte erase_y;
  byte oldBallColor; //Holds previous color of the field where the ball was drawn
 
  int bat1_y;  //bat starting y positions
  int bat2_y;
  int bat1_target_y;  //bat targets for bats to move to
  int bat2_target_y;
  byte bat1_update;  //flags - set to update bat position
  byte bat2_update;
  byte bat1miss, bat2miss; //flags set on the minute or hour that trigger the bats to miss the ball, thus upping the score to match the time.
  byte restart;   //game restart flag - set to 1 initially to setup 1st game

  byte pong_get_ball_endpoint(int tempballpos_x, int  tempballpos_y, int  tempballvel_x, int tempballvel_y);

public:
  virtual void init();
  virtual void updateDisplay();
};

#endif


