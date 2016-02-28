#include "Arduino.h"
#include "CoolingChamber.h"

CoolingChamber coolingChamber(13,2,5.00f, 0.5f);

void setup(){
  coolingChamber.start();
}

void loop(){
}
