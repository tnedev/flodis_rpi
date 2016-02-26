#include "Arduino.h"
#include "CoolingChamber.h"

CoolingChamber coolingChamber(13, 5.00f);

void setup(){
  coolingChamber.start();
}

void loop(){

}
