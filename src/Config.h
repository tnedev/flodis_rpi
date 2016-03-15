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

const int BOTTLES = 3; // Number of bottles

const int BOTTLE_RELEASE_PINS[BOTTLES] = {3,4,5}; // Pins controlling the solenoid valve for pushing out the liquid
const int BOTTLE_CLEAR_PINS[BOTTLES] = {6,7,8}; // Pins controlling the solenoid valve for clearing the remaining liquid
const int BOTTLE_CHECK_GLASS_PINS[BOTTLES] = {9,10,11}; // Pins connected with the proximity sensor underneeth each bottle

const int MINIMUM_SERVING_SIZE = 25; // the minimum serving size underneeth which the bottle is caunted as empty
const int MAX_QUANTITY = 2000;

//Cooling Chamber pin
const float ERROR_TEMP = 55.55f; // If this temperature is returned it means problem with the sensor
const int TEMP_SENSOR_PIN = 12;
const int COOLING_CHAMBER_CONTROL_PIN = 13;
const int CHECK_TEMP_CONTROL = 5000; //check the tempeature and apply control every CHECK_TEMP_COTRNOL [ms]
const float TEMP_TARGET = 23.00f;
const float TEMP_CONTROL_DELTA = 0.5f;

//EEPROM MEMORY Address allocation

const int BOTTLE_QUANTITY_ADDRESS = 1;
const int TEMP_TARGET_ADDRESS = 30;


#endif
