

#ifndef CoolingChamber_h
#define CoolingChamber_h

#include "Arduino.h"

class CoolingChamber{
  public:
    CoolingChamber(int controlPin,int tempPin, float tempTarget, float tempControlDelta);
    void start();
    void stop();
    void setTempTarger(float newTempTarget);
    static float getTemp();
    float getTempTarget();
    void setTempControlDelta(float newTempControlDelta);
    float getTempControlDelta();
    void startCooling();
    void stopCooling();

  private:
    static void checkTemp();
    static float getTempSensorData();
    float _tempTarget;
    float _tempControlDelta; // The maximum temperature difference between target and current temp before starting control
    int _controlPin;
    static int _tempSensorPin;
};

#endif
