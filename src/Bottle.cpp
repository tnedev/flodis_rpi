/* Bottle.cpp is an interpretation of a bottle in the Flow machine.
* Each bottle contains certain quantity of liquid.
* The Bottle could serve drinks, do its maintance and add new physical bottle.
* Written by Tihomir Nedev in Feb 2016
*/

#include "Arduino.h"
#include "Bottle.h"

int Bottle::_bottleQuantity[BOTTLES];

void Bottle::init(){

    for (int i = 0; i < BOTTLES; i++) {
        pinMode(BOTTLE_RELEASE_PINS[i], OUTPUT); // Set the solenoid valve pins as outputs
        pinMode(BOTTLE_CLEAR_PINS[i], OUTPUT); // Set the clearing solenoid valve pins as outputs
        pinMode(BOTTLE_CHECK_GLASS_PINS[i], INPUT); // Set the proximity sensor pins as inputs
        _bottleQuantity[i] = 0;
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

        _bottleQuantity[bottle-1]-=servingSize;
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
