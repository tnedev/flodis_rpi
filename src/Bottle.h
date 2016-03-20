/* Bottle.h is an interpretation of a bottle in the Flow machine.
* Each bottle contains certain quantity of liquid, serving sizes and price.
* The Bottle could serve drinks, do its maintance and add new physical bottle.
* Written by Tihomir Nedev in Feb 2016
*/

#ifndef Bottle_h
#define Bottle_h

#include "Arduino.h"
#include "Config.h"

class Bottle{
public:
    static void init();
    static void serve(int bottle, int servingSize); // Serve a drink of size
    static boolean drain(int bottle); // Drain the bottle
    static boolean clean(int bottle, int cleanTime); // Cleaning procedure
    static boolean checkForGlass(int bottle); // Checks if there is a glass to serve in
};

#endif
