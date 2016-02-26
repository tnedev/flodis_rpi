/* GasBottle.h is an interpretation of the pressurized gas bottle in the Flow machine
 * Written by Tihomir Nedev in Feb 2016
 */

#ifndef GasBottle_h
#define GasBottle_h

#include "Arduino.h"

class GasBottle{
  public:
    GasBottle(int pressureSensorPin, int pressureTarget);
    int getPressure();
    void setPressureTarget();
    boolean isEmpty();

  private:
    static void checkPressure();
    int _pressureTarget

};

#endif
