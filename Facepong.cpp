/*
 *********************************************************************************************************
 * AppPong.cpp, part of Wise Clock 3 library, by FlorinC (http://timewitharduino.blogspot.com/);
 *
 * Code copied and adapted from Nick (http://123led.wordpress.com/about/);
 *
 * Apr 07, 2012 (mcm)  parameterized colors
 * Apr 10, 2012 (mcm)  Honor 12hr/24hr flag
 * Apr 12, 2012 (mcm)  parameterized for X_MAX, Y_MAX, BAT_HEIGHT
 * Apr 13, 2012 (mcm)  removed floating point code in favor of scaled integers
 * Apr 14, 2012 (mcm)  fixed prediction code, flick code
 *********************************************************************************************************
 */
#include "FacePong.h"
#include "HT1632.h"

// (fc) dependent on the screen size;
#define BAT1_X 2		// Pong left bat x pos (this is where the ball collision occurs, the bat is drawn 1 behind these coords)
#define BAT2_X (X_MAX-3)	// Pong right bat x pos (this is where the ball collision occurs, the bat is drawn 1 behind these coords)

#define BAT_HEIGHT 5	// height of bat

#define COLOR_SCORE	RED
#define COLOR_PITCH	GREEN
#define COLOR_BAT	ORANGE
#define COLOR_BALL	GREEN
void PongFace::init()
{
  erase_x = 10;  //holds ball old pos so we can erase it, set to blank area of screen initially.
  erase_y = 10;
  bat1_y = 5;  //bat starting y positions
  bat2_y = 5;
  bat1_target_y = 5;  //bat targets for bats to move to
  bat2_target_y = 5;
  bat1_update = 1;  //flags - set to update bat position
  bat2_update = 1;
  restart = 1;   //game restart flag - set to 1 initially to setup 1st game

  ht1632_clear();

  // draw pitch centre line;
    for(int i = 0; i < Y_MAX; i+=2)
      ht1632_plot(X_MAX/2, i, COLOR_PITCH);
}


//*****************************************************************************************************

byte PongFace::pong_get_ball_endpoint(int tempballpos_x, int  tempballpos_y, int  tempballvel_x, int tempballvel_y)
{
  //run prediction until ball hits bat
  while (tempballpos_x > BAT1_X && tempballpos_x < BAT2_X  ){     
    tempballpos_x = tempballpos_x + tempballvel_x;
    tempballpos_y = tempballpos_y + tempballvel_y;
    //check for collisions with top / bottom
    if (tempballpos_y <= 0){
      tempballpos_y = 0;
      tempballvel_y = tempballvel_y * -1;
    } else if (tempballpos_y >= ((Y_MAX-1) * 256)){
      tempballpos_y = (Y_MAX-1) * 256;
      tempballvel_y = tempballvel_y * -1;
    }    
  }  
  return tempballpos_y >> 8; 
}


//*****************************************************************************************************
// this function is called as part of the main loop;
//
void PongFace::updateDisplay()
{
  //if restart flag is 1, setup a new game
  if (restart)
  {
    //erase ball pos
    plot (erase_x, erase_y, 0);


    /*
	char buffer[3];
     
     itoa(hours,buffer,10);
     	
     // fix - as otherwise if num has leading zero, e.g. "03" hours, itoa coverts this to chars with space "3 ".
     if (hours < 10) {
     buffer[1] = buffer[0];
     buffer[0] = '0';
     }
     	*/

    ht1632_putSmallChar( (X_MAX/2)-11, -1,('0' + hours/10) ,COLOR_SCORE);
    ht1632_putSmallChar((X_MAX/2)- 6, -1,('0' + hours % 10) , COLOR_SCORE);


    /*
    itoa(minutes,buffer,10);
     if (minutes < 10) {
     buffer[1] = buffer[0];
     buffer[0] = '0';
     }
     	*/
    ht1632_putSmallChar((X_MAX/2)+ 1, -1, ('0' + minutes / 10), COLOR_SCORE);
    ht1632_putSmallChar((X_MAX/2)+ 6, -1, ('0' + minutes % 10), COLOR_SCORE);

    // set ball start pos
    ballpos_x = X_MAX/2;
    ballpos_y = random (4,Y_MAX-4) << 8;

    // pick random ball direction
    if (random(0,2) > 0) {
      ballvel_x = 1; 
    } 
    else {
      ballvel_x = -1;
    }
    if (random(0,2) > 0) {
      ballvel_y =  random(100, 150); 
    } 
    else {
      ballvel_y = -random(100, 150);
    }
    // draw bats in initial positions
    bat1miss = 0;
    bat2miss = 0;
    // reset game restart flag
    restart = 0;

    //short wait
    delay(1500);
  }

  /*
  //if coming up to the minute: secs = 59 and mins < 59, flag bat 2 (right side) to miss the return so we inc the minutes score
   if (seconds == 59 && minutes < 59){
   bat1miss = 1;
   }
   // if coming up to the hour: secs = 59  and mins = 59, flag bat 1 (left side) to miss the return, so we inc the hours score.
   if (seconds == 59 && minutes == 59){
   bat2miss = 1;
   }
   */
  if (seconds == 59) {
    if (minutes == 59) {
      bat2miss = 1;
    }
    else {
      bat1miss = 1;
    }


    //AI - we run 2 sets of 'AI' for each bat to work out where to go to hit the ball back 

    // Left bat AI
    // when the ball is closer to the left bat and heading that way, run the ball maths to find out where the ball will land
    if (ballpos_x == ((X_MAX/2)-1) && ballvel_x < 0) {
      byte end_ball_y = pong_get_ball_endpoint(ballpos_x, ballpos_y, ballvel_x, ballvel_y);

      //if the miss flag is set,  then the bat needs to miss the ball when it gets to end_ball_y
      if (bat1miss){
        if (end_ball_y > (Y_MAX/2)){
	  bat1_target_y = end_ball_y - BAT_HEIGHT - random (1,3);	// near miss appearent low part of bat
	} else {
	  bat1_target_y = end_ball_y + random (2,4);			// near miss appearent high part of bat
	}      
      } else {
        //if the miss flag isn't set,  set bat target to ball end point with some randomness so its not always hitting top of bat
	bat1_target_y = end_ball_y - random(0,BAT_HEIGHT);        
	//check not less than 0
	if (bat1_target_y < 0){
	  bat1_target_y = 0;
	} else if (bat1_target_y > (Y_MAX - BAT_HEIGHT)){
	  bat1_target_y = (Y_MAX - BAT_HEIGHT);
	} 
      }
    }
    // When further away, just tell the bat to move to the height of the ball when we get to a random location.
    else if (ballpos_x == random(X_MAX/2 + 4, X_MAX/2 + 11)){// && ballvel_x < 0) {
      bat1_target_y = ballpos_y >> 8;
    }
        
        
    // Right bat AI
    // when the ball is closer to the right bat and heading that way, run the ball maths to find out where the ball will land
    if (ballpos_x == ((X_MAX/2)+1) && ballvel_x > 0) {
      byte end_ball_y = pong_get_ball_endpoint(ballpos_x, ballpos_y, ballvel_x, ballvel_y);

      //if flag set to miss, move bat out way of ball
      if (bat2miss){
        //if ball end point above 8 then move bat down, else move it up- so either way it misses
        if (end_ball_y > (Y_MAX/2)){
          bat2_target_y = end_ball_y - BAT_HEIGHT - random (1,3);	// near miss appearent low part of bat 
        } else {
          bat2_target_y = end_ball_y + random (2,4);			// near miss appearent high part of bat
        }      
      } else {
        //set bat target to ball end point with some randomness 
        bat2_target_y = end_ball_y - random (0,BAT_HEIGHT);
        //ensure target between 0 and 15
        if (bat2_target_y < 0){
          bat2_target_y = 0;
        } else if (bat2_target_y > (Y_MAX - BAT_HEIGHT)){
          bat2_target_y = (Y_MAX - BAT_HEIGHT);
        } 
      }
    }
    // When further away, just tell the bat to move to the height of the ball when we get to a random location.
    else if (ballpos_x == random(X_MAX/2 - 11, X_MAX/2 - 4)){//  && ballvel_x > 0) {
      bat2_target_y = ballpos_y >> 8;
    }


    //move bat 1 towards target    
    //if bat y greater than target y move down until hit 0 (dont go any further or bat will move off screen)
    if (bat1_y > bat1_target_y) {
#if Y_MAX > X_MAX/2
      if ((ballvel_x < 0) && (ballpos_x - BAT1_X) < (bat1_y - bat1_target_y))
        bat1_y -= 2;
      else
#endif
        bat1_y--;
      if (bat1_y < 0) bat1_y = 0;
      bat1_update = 1;
    }

    //if bat y less than target y move up until hit 10 (as bat is 6)
    else if (bat1_y < bat1_target_y) {
#if Y_MAX > X_MAX/2
      if ((ballvel_x < 0) && (ballpos_x - BAT1_X) < (bat1_target_y - bat1_y))
        bat1_y += 2;
      else
#endif
        bat1_y++;
      if (bat1_y > (Y_MAX - BAT_HEIGHT))
        bat1_y = (Y_MAX - BAT_HEIGHT);
      bat1_update = 1;
    }

    //draw bat 1
    if (bat1_update){
      for (byte i = 0; i < Y_MAX; i++){
        if (i - bat1_y < BAT_HEIGHT &&  i - bat1_y > -1){
          plot(BAT1_X-1, i , COLOR_BAT);
          plot(BAT1_X-2, i , COLOR_BAT);
        } else {
          plot(BAT1_X-1, i , 0);
          plot(BAT1_X-2, i , 0);
        }
      } 
    }


    //move bat 2 towards target (dont go any further or bat will move off screen)

    //if bat y greater than target y move down until hit 0
    if (bat2_y > bat2_target_y) {
#if Y_MAX > X_MAX/2
      if ((ballvel_x > 0) && (BAT2_X - ballpos_x) < (bat2_y - bat2_target_y))
        bat2_y -= 2;
      else
#endif
        bat2_y--;
      if (bat2_y < 0) bat2_y = 0;
      bat2_update = 1;
    }

    //if bat y less than target y move up until hit max of 10 (as bat is 6)
    else if (bat2_y < bat2_target_y) {
#if Y_MAX > X_MAX/2
      if ((ballvel_x > 0) && (BAT2_X - ballpos_x) < (bat2_target_y - bat2_y))
        bat2_y += 2;
      else
#endif
        bat2_y++;
      if (bat2_y > (Y_MAX - BAT_HEIGHT))
        bat2_y = (Y_MAX - BAT_HEIGHT);
      bat2_update = 1;
    }

    //draw bat2
    if (bat2_update){
      for (byte i = 0; i < Y_MAX; i++){
        if (i - bat2_y < BAT_HEIGHT && i - bat2_y > -1){
          plot(BAT2_X+1, i , COLOR_BAT);
          plot(BAT2_X+2, i , COLOR_BAT);
        } 
        else {
          plot(BAT2_X+1, i , 0);
          plot(BAT2_X+2, i , 0);
        }
      } 
    }

    //update the ball position using the velocity
    ballpos_x =  ballpos_x + ballvel_x;
    ballpos_y =  ballpos_y + ballvel_y;

    //check ball collision with top and bottom of screen and reverse the y velocity if either is hit
    if (ballpos_y <= 0 ){
      ballvel_y = ballvel_y * -1;
      ballpos_y = 0; //make sure value goes no less that 0
    } 
    else if (ballpos_y >= ((Y_MAX-1) * 256)){
      ballvel_y = ballvel_y * -1;
      ballpos_y = (Y_MAX-1) * 256; //make sure value goes no more than 15
    }

    //check for ball collision with bat1. check ballx is same as batx
    //and also check if bally lies within width of bat i.e. baty to baty + 6. We can use the exp if(a < b && b < c)
    // Update code to reduce size Quentin Smart Mar 12

    if (ballpos_x == BAT1_X && (bat1_y <= (ballpos_y >> 8) && (ballpos_y >> 8) <= bat1_y + (BAT_HEIGHT-1)) ) { 

      ballvel_x = ballvel_x * -1;  // Happens irrespective

      //random if bat flicks ball to return it - and therefor changes ball velocity
      if(random(0,3)) { //true = flick 
        bat1_update = 1;
        byte flick = 0;  //0 = up, 1 = down.

        //    if (bat1_y > 1 || bat1_y < Y_MAX/2){
        //      flick = random(0,2);   //pick a random dir to flick - up or down
        //    }

        //if bat 1 or 2 away from top only flick down
        // if (bat1_y <=1 ){  (Flick is 0 anyway..  //move bat down 1 or 2 pixels
        //  flick = 0;   

        //if bat 1 or 2 away from bottom only flick up
        if (bat1_y >=  (Y_MAX-BAT_HEIGHT-1) ){
          flick = 1;  //move bat up 1 or 2 pixels
        }

        if (flick == 0) {
          //flick up
          bat1_target_y += random(1,3);
          if (ballvel_y < 0 && ballvel_y > -500)
            ballvel_y -= random(30, 51);
          else if (ballvel_y >= 0 && ballvel_y < 500)
            ballvel_y += random(30, 51);
        }               
        //flick down
        else {
          bat1_target_y -= random(1,3);
          if (ballvel_y < -50)
            ballvel_y += random(30, 51);
          else if (ballvel_y > 50)
            ballvel_y -= random(30, 51);
        }
      }
    }
  }


  //check for ball collision with bat2. check ballx is same as batx
  //and also check if bally lies within width of bat i.e. baty to baty + 6. We can use the exp if(a < b && b < c)
  if (ballpos_x == BAT2_X && (bat2_y <= (ballpos_y >> 8) && (ballpos_y >> 8) <= bat2_y + (BAT_HEIGHT-1)) ) { 
    ballvel_x = ballvel_x * -1;   // Happens irrespective
    //random if bat flicks ball to return it - and therefor changes ball velocity
    if(random(0,3)) {  // true flick -
      bat2_update = 1;
      byte flick = 0;  //0 = up, 1 = down.

      if (bat2_y > 1 || bat2_y < (Y_MAX-BAT_HEIGHT)){
        flick = random(0,2);   //pick a random dir to flick - up or down
      }
      //if bat 1 or 2 away from top only flick down
      //      if (bat2_y <= 1 ){
      //        flick = 0;  //move bat up 1 or 2 pixels
      //      }
      //if bat 1 or 2 away from bottom only flick up
      if (bat2_y >=  Y_MAX/2 ){
        flick = 1;   //move bat down 1 or 2 pixels
      }

      if (flick == 0) {
        //flick up
        bat2_target_y += random(1,3);

        if (ballvel_y < 0 && ballvel_y > -500)
          ballvel_y -= random(30, 51);
        else if (ballvel_y >= 0 && ballvel_y < 500)
          ballvel_y += random(30, 51);
      }

      //flick down
      else { 
        bat2_target_y -= random(1,3);
        if (ballvel_y < -50)
          ballvel_y += random(30, 51);
        else if (ballvel_y > 50)
          ballvel_y -= random(30, 51);

      }

    }
  } 

  //plot the ball on the screen
  byte plot_x = ballpos_x;
  byte plot_y = ballpos_y >> 8;

  //take a snapshot of all the led states
  snapshot_shadowram();

  //set prev. pixel back to old color
  plot(erase_x, erase_y, oldBallColor);
  //Get current color of the pixel the ball will be drawn to
  oldBallColor = get_shadowram(plot_x, plot_y);
  //Draw the ball
  plot(plot_x, plot_y, ORANGE);

  //reset erase to new pos
  erase_x = plot_x;
  erase_y = plot_y;

  //check if a bat missed the ball. if it did, reset the game.
  if (ballpos_x <= (BAT1_X-2) || ballpos_x >= (BAT2_X+2)){
    restart = 1;
  }
  delay(35);

  //  fade_down();
}





