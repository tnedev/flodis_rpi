/* Bottle.h is an interpretation of a bottle in the Flow machine.
* Each bottle contains certain quantity of liquid, serving sizes and price.
* The Bottle could serve drinks, do its maintance and add new physical bottle.
* Written by Tihomir Nedev in Feb 2016
*/

#ifndef Bottle_h
#define Bottle_h

#include "Arduino.h"

class Bottle{
public:
    Bottle(int controlPins[2], int bottleSelectorPin);
    void newBottle(int quantity, int servingSizes[5], float price[5]);
    int serve(int servingSize); // Serve a drink of size
    int drain(); // Drain the bottle
    int clean(int cleanTime); // Cleaning procedure
    void setServingSizes(int newServingSizes[5]);
    int getServingSizes();
    int getQuantity();
    void setQuantity(int newQuantity); // Set the quantity of liquid in the bottle
    boolean hasGlass(); // Checks if there is a glass to serve in
    boolean isEmpty();


private:
    int _controlPins[2];
    // There are two controling pins. Pin 1 for pushing the required drink size through the control valve.
    // Pin 2 for clearing out the pipe from leftover liquid.
    int _bottleSelectorPin; // This is the pin connected with the proximity sensor
    int _quantity; // The quantity of liquid inside the bottle
    int _servingSizes[5]; // Serving sized
    char* _type; // Type of drink in the bottle

};

#endif
