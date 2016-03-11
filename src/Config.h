/* Config.h is an configuration for all Arduino pins and communication commands,
* Use it to set the pin numbers or define a new command.
 */

 #ifndef Config_h
 #define Config_h

// I2C RPi -> Arduino communication. Slave address of the Arduino
const int SLAVE_ADDRESS = 0x04;

 /*
    Arduino pinout configuration.
 */

const int bottles = 8;
 // Pins controlling the gas release valve
const int BOTTLE1_CONTROL_PINS[2] = {2,4};
const int BOTTLE2_CONTROL_PINS[2] = {3,5};

// Pins of the proximity sensors underneeth each nozzle
const int BOTTLE1_SELECTOR_PIN = 5;
const int BOTTLE2_SELECTOR_PIN = 6;


//Cooling Chamber pin
const float ERROR_TEMP = 55.55f; // If this temperature is returned it means problem with the sensor
const int TEMP_SENSOR_PIN = 12;
const int COOLING_CHAMBER_CONTROL_PIN = 10;
const int CHECK_TEMP_CONTROL = 5000; //check the tempeature and apply control every CHECK_TEMP_COTRNOL [ms]
const float TEMP_TARGET = 23.00f;
const float TEMP_CONTROL_DELTA = 0.5f;


 #endif
