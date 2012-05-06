#include "ClockFace.h"
#include "HT1632.h"

class SimpleFace: 
public ClockFace {
private:
  long dotMillis;
  bool dotsVisible;
  int isTimeSetting; 

public:
  virtual void init();
  virtual void updateDisplay();
  virtual void setPressed();
  virtual void plusPressed();

};

