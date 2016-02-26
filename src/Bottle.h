/* Bottle.h is an interpretation of a bottle in the Flow machine.
 * Each bottle contains certain quantity of liquid, drink sizes and price.
 * The Bottle could serve drinks, do its maintance and add new physical bottle.
 * Written by Tihomir Nedev in Feb 2016
 */

#ifndef Bottle_h
#define Bottle_h

#include "Arduino.h"

class Bottle{
  public:
    Bottle(int controlPins[2]);
    newBottle(int quantity, float price[][])
    int serve(int servingSize);
    int drain();
    int clean(int cleanTime);
    float[][] getPrice();
    void setPrice(float newPrice[][]);
    int getQuantity();
    void setQuantity(int newQuantity);
    boolean hasGlass(); // Checks if there is a glass to serve in
    boolean isEmpty();


  private:
    int _controlPins[2];
    int _quantity;
    float _price[][];
    char* _type;

};

#endif
