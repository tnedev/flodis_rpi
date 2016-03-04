/* CoolingChamber.cpp is an interpretation of the cooling chamber on the Flow machine.
 * It provides a temperature control for the chamber and interface for itneraction with it.
 * Written by Tihomir Nedev in Feb 2016
 */

#include "Arduino.h"
#include "MsTimer2.h"
#include "OneWire.h"
#include "CoolingChamber.h"

int CoolingChamber::_tempSensorPin = -1;

CoolingChamber::CoolingChamber(int controlPin, int tempSensorPin, float tempTarget, float tempControlDelta){
  _controlPin = controlPin;
  _tempSensorPin = tempSensorPin;
  _tempControlDelta = tempControlDelta;
  _tempTarget = tempTarget;

  pinMode(_controlPin, OUTPUT);
  MsTimer2::set(5000, checkTemp); // check the temperature at this time
  //TODO: Imlement the actual control of the chamber
}

void CoolingChamber::start(){
  MsTimer2::start();
}

void CoolingChamber::stop(){
  MsTimer2::stop();
}

void CoolingChamber::setTempTarger(float newTempTarget){
  _tempTarget = newTempTarget;
}

float CoolingChamber::getTempTarget(){
  return _tempTarget;
}

void CoolingChamber::checkTemp(){
  // Implement the control here

}

float CoolingChamber::getTemp(){
  return getTempSensorData();
}

float CoolingChamber::getTempControlDelta(){
  return _tempControlDelta;
}

void CoolingChamber::setTempControlDelta(float newTempControlDelta){
  _tempControlDelta = newTempControlDelta;
}

float CoolingChamber::getTempSensorData(){
  //returns the temperature from one tempSensor18S20 in DEG Celsius
  // TODO: Add error messages to MessageQ

   static OneWire _tempSensor(_tempSensorPin);
   byte data[12];
   byte addr[8];

   if ( !_tempSensor.search(addr)) {
     //no more sensors on chain, reset search
     _tempSensor.reset_search();
     return -1000;
   }

   if ( OneWire::crc8( addr, 7) != addr[7]) {
    // ("CRC is not valid!");
     return -1000;
   }

   if ( addr[0] != 0x10 && addr[0] != 0x28) {
     //("Device is not recognized");
     return -1000;
   }

   _tempSensor.reset();
   _tempSensor.select(addr);
   _tempSensor.write(0x44,1); // start conversion, with parasite power on at the end

   byte present = _tempSensor.reset();
   _tempSensor.select(addr);
   _tempSensor.write(0xBE); // Read Scratchpad


   for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = _tempSensor.read();
   }

   _tempSensor.reset_search();

   byte MSB = data[1];
   byte LSB = data[0];

   float tempRead = ((MSB << 8) | LSB); //using two's compliment
   float TemperatureSum = tempRead / 16;

   return TemperatureSum;
}

void CoolingChamber::startCooling(){

}

void CoolingChamber::stopCooling(){

}
