/* CoolingChamber.cpp is an interpretation of the cooling chamber on the Flow machine.
* It provides a temperature control for the chamber and interface for itneraction with it.
* Written by Tihomir Nedev in Feb 2016
*/

#include "Arduino.h"
#include "MsTimer2.h"
#include "OneWire.h"
#include "EEPROMex.h"
#include "CoolingChamber.h"
#include "Config.h"

int tempCheckTimer = CHECK_TEMP_CONTROL;
int CoolingChamber::_controlPin;
int CoolingChamber::_tempSensorPin;
float CoolingChamber::_tempTarget;
float CoolingChamber::_tempControlDelta;
float CoolingChamber::currentTemp;
boolean CoolingChamber::_isCooling;
boolean CoolingChamber::_isResting;

void CoolingChamber::init(){
    _controlPin = COOLING_CHAMBER_CONTROL_PIN;
    _tempSensorPin = TEMP_SENSOR_PIN;
    _tempControlDelta = TEMP_CONTROL_DELTA;
    _tempTarget = EEPROM.readFloat(TEMP_TARGET_ADDRESS);

    currentTemp = getTempSensorData();
    pinMode(_controlPin, OUTPUT);
    MsTimer2::set(tempCheckTimer, checkTemp); // check the temperature at this time
}

void CoolingChamber::start(){
    MsTimer2::start();
}

void CoolingChamber::stop(){
    MsTimer2::stop();
    stopCooling();
    _isCooling = false;
    _isResting = false;
}

void CoolingChamber::setTempTarger(float newTempTarget){
    _tempTarget = newTempTarget;
    EEPROM.updateFloat(TEMP_TARGET_ADDRESS, _tempTarget);
}

float CoolingChamber::getTempTarget(){
    return _tempTarget;
}

void CoolingChamber::checkTemp(){
    // Implement the control here
    currentTemp = getTempSensorData();
    if (currentTemp >= (_tempTarget + ( _tempControlDelta/2))){
        startCooling();
        if(!_isCooling){
            _isCooling = true;
            _isResting = false;
            Serial.println("Started Cooling Process");
        }
    }
    else if (currentTemp < ( _tempTarget -( _tempControlDelta/2))){
        stopCooling();
        if(!_isResting){
            _isCooling = false;
            _isResting = true;
            Serial.println("Stopped Cooling Process");
        }
    }
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

    if (TemperatureSum >-9.99f && TemperatureSum < 45.00f){
        Serial.print("Temperature is: ");
        Serial.println(TemperatureSum);
        return TemperatureSum;
    }
    else {
        return ERROR_TEMP;
        Serial.println("Temperature sensor problem");
    }
}

void CoolingChamber::startCooling(){

}

void CoolingChamber::stopCooling(){

}
