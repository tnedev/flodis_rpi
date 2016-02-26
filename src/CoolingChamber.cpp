/* CoolingChamber.cpp is an interpretation of the cooling chamber on the Flow machine.
 * It provides a temperature control for the chamber and interface for itneraction with it.
 * Written by Tihomir Nedev in Feb 2016
 */

#include "Arduino.h"
#include <MsTimer2.h>
#include "CoolingChamber.h"


CoolingChamber::CoolingChamber(int pin, float tempTarget){
  _pin = pin;
  pinMode(_pin, OUTPUT);
  MsTimer2::set(5000, checkTemp);

  _tempTarget = tempTarget;

  //TODO: Imlement the actual control of the chamber
  // TODO: Implement the temperature sensor
}

void CoolingChamber::start(){
  MsTimer2::start();
}

void CoolingChamber::stop(){
  MsTimer2::stop();
}

void CoolingChamber::setTempTarger(float tempTarget){
  tempTarget = _tempTarget;
}

float CoolingChamber::getTempTarget(){
  return _tempTarget;
}

void CoolingChamber::checkTemp(){
  // Only used to check the functionality. To be deleted.
  static boolean output = false ;
  digitalWrite(13, output);
  output = !output;
}

float CoolingChamber::getTemp(){
  return getTempSensorData();
}

float CoolingChamber::getTempSensorData(){
  //TODO: implement the sensor data
}
