/* Config.h is an configuration for all Arduino pins and communication commands,
* Use it to set the pin numbers or define a new command.
*/

#ifndef RPiComm_h
#define RPiCom_h

#include "Arduino.h"


const int SERIAL_TIMEOUT = 250; //timeout before cancel the message [ms]
const int SERIAL_REFRESH_TIMER = 30000;

const char START_BYTE = '[';
const char END_BYTE = ']';
const char SPLIT_BYTE = '&';
const char VALUES_BYTE = ':';

///////////////////////////////////////
const int HEARTBEAT_VAL = 1;
//  Command start strings
const String ERROR_MSG = "Err:";
const String HEARTBEAT_MSG = "hb:";
const String SET_TEMP_TARGET_MSG = "stt:";
const String GET_TEMP_TARGET_MSG = "gtt:";
const String GET_TEMP_MSG = "gt:";
const String CHECK_GLASS_MSG = "cfg:";
const String GET_PRESSURE_MSG = "gp:";
const String SERVE_DRINK_MSG = "sd:";
const String STOP_OPERATION_MSG = "co:";
const String START_OPERATION_MSG = "so:";
const String STOP_COOLING_MSG = "cc:";
const String START_COOLING_MSG = "sc:";


// ArduinoCommunicationError
const int ERR_ARDUINO_READ_TIMEOUT = 101;

// ArduinoBadMessageError
const int ERR_NON_EMPTY_BUFF = 202; // Trying to start writing on a non-empty buffer
const int ERR_CLOSE_EMPTY_BUFF = 203; // Trying to close a message on an empty buffer
const int ERR_NO_START_BYTE = 204; // Writing a message before a start byte
const int ERR_BAD_MESSAGE = 205;

// ArduinoBadArgumentsError
const int ERR_BAD_VALUE = 31; // When an argument is not the right format example float instead of int
const int ERR_BOTTLE_NUMBER = 32; // Wrong bottle number; Out of the range of the declared bottles
const int ERR_SERVING_TIME = 33; // Serving time was wrong
const int ERR_TEMP_TARGET = 34;
const int ERR_BOTTLE_BUSY = 35; 

// ArduinoSensorError
const int ERR_TEMP_SENSOR = 15; // There is something wrong with the sensor
const int ERR_PRESSURE_SENSOR = 16; // Something wrong with the pressure sensor

// DispenserStoppedWorkingError
const int ERR_LOW_PRESSURE = 115; // A low pressure in the gas bottle -> It"s empty
const int ERR_HIGH_PRESSURE = 116; // High pressure in tge gas bottle -> Not set write
const int ERR_LOW_TEMP = 117; // The temperature is to low
const int ERR_HIGH_TEMP = 118;


/////////////////

void sendErrorMessage(int value);
void sendExcMessage(int value);
void sendSetMessageResp(String setting, int value);
void sendSetMessageResp(String setting, int value, int extrValue);
void sendGetMessageResp(String get_message, int value);
void sendGetMessageResp(String get_message, int value, int extrValue);
void sendLogMessage(String message);
bool parseCommandParams(String data_str, String init_command, int results[], int arr_length);

#endif
