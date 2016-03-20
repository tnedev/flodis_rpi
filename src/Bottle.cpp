/* Bottle.cpp is an interpretation of a bottle in the Flow machine.
* Each bottle contains certain quantity of liquid.
* The Bottle could serve drinks, do its maintance and add new physical bottle.
* Written by Tihomir Nedev in Feb 2016
*/

#include "Arduino.h"
#include "Bottle.h"
#include "EEPROMex.h"
#include "RPiComm.h"

void Bottle::init(){
    for (int i = 0; i < BOTTLES; i++) {
        pinMode(BOTTLE_RELEASE_PINS[i], OUTPUT); // Set the solenoid valve pins as outputs
        pinMode(BOTTLE_CLEAR_PINS[i], OUTPUT); // Set the clearing solenoid valve pins as outputs
        pinMode(BOTTLE_CHECK_GLASS_PINS[i], INPUT); // Set the proximity sensor pins as inputs
    }
}

boolean Bottle::checkForGlass(int bottle){
    return !digitalRead(BOTTLE_CHECK_GLASS_PINS[bottle-1]);
}

void Bottle::serve(int bottle, int servingTime){

            digitalWrite(BOTTLE_RELEASE_PINS[bottle-1], HIGH);
            delay(servingTime);
            digitalWrite(BOTTLE_RELEASE_PINS[bottle-1], LOW);
            digitalWrite(BOTTLE_CLEAR_PINS[bottle-1], HIGH);
            delay(300); //clear the remaining liquid
            digitalWrite(BOTTLE_CLEAR_PINS[bottle-1], LOW);
}
