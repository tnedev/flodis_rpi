/* Bottle.cpp is an interpretation of a bottle in the Flow machine.
* Each bottle contains certain quantity of liquid.
* The Bottle could serve drinks, do its maintance and add new physical bottle.
* Written by Tihomir Nedev in Feb 2016
*/

#include "Arduino.h"
#include "Bottle.h"
#include "EEPROMex.h"

int Bottle::_bottleQuantity[BOTTLES];

void Bottle::init(){
    EEPROM.readBlock<int>(BOTTLE_QUANTITY_ADDRESS, _bottleQuantity, BOTTLES); // Write the bottle quantities to memorie
    for (int i = 0; i < BOTTLES; i++) {
        pinMode(BOTTLE_RELEASE_PINS[i], OUTPUT); // Set the solenoid valve pins as outputs
        pinMode(BOTTLE_CLEAR_PINS[i], OUTPUT); // Set the clearing solenoid valve pins as outputs
        pinMode(BOTTLE_CHECK_GLASS_PINS[i], INPUT); // Set the proximity sensor pins as inputs
    }
}

boolean Bottle::newBottle(int bottle, int quantity){
    if (bottle<=BOTTLES && quantity<=MAX_QUANTITY){
        _bottleQuantity[bottle-1] = quantity;
        return true;
    }
    else {
        return false;
    }

}


int Bottle::getQuantity(int bottle){
    if(bottle <= BOTTLES){
        return _bottleQuantity[bottle-1];
    }
    else {
        return -1;
    }
}

boolean Bottle::setQuantity(int bottle, int newQuantity){
    if (bottle<=BOTTLES && newQuantity<=MAX_QUANTITY){
        _bottleQuantity[bottle-1] = newQuantity;
        EEPROM.updateBlock<int>(BOTTLE_QUANTITY_ADDRESS, _bottleQuantity, BOTTLES);
        return true;
    }
    else {
        return false;
    }
}

boolean Bottle::checkForGlass(int bottle){
    return digitalRead(BOTTLE_CHECK_GLASS_PINS[bottle-1]);
}

boolean Bottle::serve(int bottle, int servingSize){

    if (bottle<=BOTTLES && _bottleQuantity[bottle-1]>=servingSize){
        // TODO:Serving procedure
        digitalWrite(BOTTLE_RELEASE_PINS[bottle-1], HIGH);
        delay(servingSize*55);
        digitalWrite(BOTTLE_RELEASE_PINS[bottle-1], LOW);
        digitalWrite(BOTTLE_CLEAR_PINS[bottle-1], HIGH);
        delay(500);
        digitalWrite(BOTTLE_CLEAR_PINS[bottle-1], LOW);
        _bottleQuantity[bottle-1]-=servingSize;
        EEPROM.updateBlock<int>(BOTTLE_QUANTITY_ADDRESS, _bottleQuantity, BOTTLES);
        return true;
    }
    else {
        return false;
    }

}



boolean Bottle::isEmpty(int bottle){
    if (_bottleQuantity[bottle-1]<MINIMUM_SERVING_SIZE){
        return true;
    }
    else {
        return false;
    }
}
