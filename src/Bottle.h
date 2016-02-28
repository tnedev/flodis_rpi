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
    int serve(int servingSize);
    int drain();
    int clean(int cleanTime);
    void setServingSizes(int newServingSizes[5]);
    int getServingSizes();
    float getPrice();
    void setPrice(float newPrice[5]);
    int getQuantity();
    void setQuantity(int newQuantity);
    boolean hasGlass(); // Checks if there is a glass to serve in
    boolean isEmpty();


  private:
    int _controlPins[2];
    int _bottleSelectorPin;
    int _quantity;
    float _price[5];
    int _servingSizes[5];
    char* _type;

};

#endif
