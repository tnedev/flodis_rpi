

#ifndef CoolingChamber_h
#define CoolingChamber_h

#include "Arduino.h"

class CoolingChamber{
  public:
    CoolingChamber(int pin, float tempTarget);
    void start();
    void stop();
    void setTempTarger(float tempTarget);
    int getTempTarget();
    static void checkTemp();

  private:
    float _tempTarget;
    int _pin;
};

#endif
