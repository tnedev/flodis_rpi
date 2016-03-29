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

const int BOTTLES = 1; // Number of bottles
const int BOTTLE_FLUSH_TIME = 300; 
const int BOTTLE_RELEASE_PINS[BOTTLES] = {8}; // Pins controlling the solenoid valve for pushing out the liquid
const int BOTTLE_CLEAR_PINS[BOTTLES] = {7}; // Pins controlling the solenoid valve for clearing the remaining liquid
const int BOTTLE_CHECK_GLASS_PINS[BOTTLES] = {6}; // Pins connected with the proximity sensor underneeth each bottle


//Cooling Chamber pin
const float ERROR_TEMP = 55.55f; // If this temperature is returned it means problem with the sensor
const int TEMP_SENSOR_PIN = 12;
const int COOLING_CHAMBER_CONTROL_PIN = 13;
const int CHECK_TEMP_CONTROL = 5000; //check the tempeature and apply control every CHECK_TEMP_COTRNOL [ms]
const float TEMP_TARGET = 23.00f;
const float TEMP_CONTROL_DELTA = 0.5f;

// Argument Ranges
const int SERVING_TIME_MAX =  20000;
const float TEMP_TARGET_MIN = 0.5; // Temp target under that temperature is not allowed
const float TEMP_TARGET_MAX = 30.0; // Temp target over that temperature is not allowed
const float TEMP_MIN = 0.1; // Minimum temperature allowed
const float TEMP_MAX =  32; // Maximum temperature allowed
const int PRESSURE_MIN = 100; // The minimum allowed pressure. Under this means the gas bottle is empty
const int PRESSURE_MAX = 300; // The maximum allowed pressure. Over this means pressure problem

//EEPROM MEMORY Address allocation

const int TEMP_TARGET_ADDRESS = 30;

#endif
