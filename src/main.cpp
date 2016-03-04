#include "Arduino.h"
#include "CoolingChamber.h"
#include "Bottle.h"
#include "Commander.h"
#include "Wire.h"

CoolingChamber coolingChamber(13,2,5.00f, 0.5f);


void setup(){
  Commander commander;
  coolingChamber.start();

}

void loop(){
}
