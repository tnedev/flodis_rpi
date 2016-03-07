

#ifndef CoolingChamber_h
#define CoolingChamber_h

#include "Arduino.h"

class CoolingChamber{
  public:
    static void init(int controlPin,int tempPin, float tempTarget, float tempControlDelta);
    static void start();
    static void stop();
    static void setTempTarger(float newTempTarget);
    static float getTemp();
    static float getTempTarget();
    static void setTempControlDelta(float newTempControlDelta);
    static float getTempControlDelta();
    static void startCooling();
    static void stopCooling();
    static void checkTemp();
    static float getTempSensorData();
    
    static float _tempTarget;
    static float _tempControlDelta; // The maximum temperature difference between target and current temp before starting control
    static int _controlPin;
    static int _tempSensorPin;
};

#endif
