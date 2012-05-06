#include "ClockFace.h"
#include "HT1632.h"

class AnalogFace: 
public ClockFace {
public:
  virtual void init();
  virtual void updateDisplay();

};

