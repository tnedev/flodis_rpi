/* Bottle.cpp is an interpretation of a bottle in the Flow machine.
* Each bottle contains certain quantity of liquid.
* The Bottle could serve drinks, do its maintance and add new physical bottle.
* Written by Tihomir Nedev in Feb 2016
*/

#include "Arduino.h"
#include "Bottle.h"
#include "EEPROMex.h"
#include "RPiComm.h"

boolean Bottle::_isFlushing[BOTTLES];
boolean Bottle::_isServing[BOTTLES];
int Bottle::_servingTime[BOTTLES];
unsigned long Bottle::_bottleTimer[BOTTLES];

void Bottle::init(){

    for (int i = 0; i < BOTTLES; i++) {
        pinMode(BOTTLE_RELEASE_PINS[i], OUTPUT); // Set the solenoid valve pins as outputs
        pinMode(BOTTLE_CLEAR_PINS[i], OUTPUT); // Set the clearing solenoid valve pins as outputs
        pinMode(BOTTLE_CHECK_GLASS_PINS[i], INPUT); // Set the proximity sensor pins as inputs
        Bottle::_isFlushing[i] = 0;
        Bottle::_isServing[i] = 0;
        Bottle::_servingTime[i] = 0;
        _bottleTimer[i] = 0;
    }
}

boolean Bottle::checkForGlass(int bottle){
    return !digitalRead(BOTTLE_CHECK_GLASS_PINS[bottle-1]);
}

void Bottle::serve(int bottle, int servingTime){

    _servingTime[bottle-1] = servingTime;
    _isServing[bottle-1] = true;
    _bottleTimer[bottle-1] = millis();
}

boolean Bottle::isServing(int bottle){
    return _isServing[bottle-1];
}

void Bottle::checkServingTime(){
    for (int i = 0; i < BOTTLES; i++) {

        if(_isServing[i]){
            digitalWrite(BOTTLE_RELEASE_PINS[i], HIGH);
            if(millis() - _bottleTimer[i] >= _servingTime[i]){
                digitalWrite(BOTTLE_RELEASE_PINS[i], LOW);
                _isServing[i] = false;
                _isFlushing[i] = true;
                _bottleTimer[i] = millis();
            }
        }
        if(_isFlushing[i]){
            digitalWrite(BOTTLE_CLEAR_PINS[i], HIGH);
            if (millis() - _bottleTimer[i] >= BOTTLE_FLUSH_TIME){
                digitalWrite(BOTTLE_CLEAR_PINS[i], LOW);
                _isFlushing[i] = false;
            }
        }
    }
}
