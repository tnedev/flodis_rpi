/* Bottle.cpp is an interpretation of a bottle in the Flow machine.
* Each bottle contains certain quantity of liquid, serving sizes and price.
* The Bottle could serve drinks, do its maintance and add new physical bottle.
* Bottle:Bottle() sets up the workings behind.
* Call Bottle:newBottle(int quantity, int servingSizes[5], float price[5]) for adding a drink bottle
* Written by Tihomir Nedev in Feb 2016
*/

#include "Arduino.h"
#include "Bottle.h"

Bottle::Bottle(int controlPins[2], int bottleSelectorPin){

    _controlPins[0] = controlPins[0];
    _controlPins[1] = controlPins[1];
    _bottleSelectorPin = bottleSelectorPin;

    pinMode(_controlPins[0], OUTPUT);
    pinMode(_controlPins[1], OUTPUT);
    pinMode(_bottleSelectorPin, OUTPUT);

}

boolean Bottle::hasGlass(){
    // Returns the value of the proximity sensor (true or false)
    return digitalRead(_bottleSelectorPin);
}

boolean Bottle::isEmpty(){
    // Check if the quantity in the bottle is smaller than the smalles serving size
    return ( _quantity < _servingSizes[0] );
}
