

#ifndef CoolingChamber_h
#define CoolingChamber_h

#include "Arduino.h"

class CoolingChamber{
public:
    static void init();
    static void start();
    static void stop();
    static void setTempTarger(float newTempTarget);
    static float getTempTarget();
    static void setTempControlDelta(float newTempControlDelta);
    static float getTempControlDelta();
    static void startCooling();
    static void stopCooling();
    static void checkTemp();
    static float getTempSensorData();

    static float currentTemp;

    static boolean _isCooling;
    static boolean _isResting;
    static float _tempTarget;
    static float _tempControlDelta; // The maximum temperature difference between target and current temp before starting control
    static int _controlPin;
    static int _tempSensorPin;

};

#endif
