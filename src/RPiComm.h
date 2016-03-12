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

const String HEARTBEAT_MSG = "hb:";
const int HEARTBEAT_VAL = 1;
const String SET_TEMP_TARGET_MSG = "stt:";
const String GET_TEMP_TARGET_MSG = "gtt:";
const String GET_TEMP_MSG = "gt:";
const String CHECK_GLASS_MSG = "cfg:";
const String GET_BOTTLE_QUANT_MSG = "gbq:";
const String SET_BOTTLE_QUANT_MSG = "sbq:";
const String GET_PRESSURE_MSG = "gp:";
const String PREV_DRINK_MSG = "sd:";

const int ERR_BAD_REQUEST_VAL = 201;
const int ERR_NON_EMPTY_BUFF = 202; //Can't start on non-empty buffer
const int ERR_CLOSE_EMPTY_BUFF = 203; //Can't close empty buffer
const int ERR_NO_START_BYTE = 204; //Can't write to buffer before start byte
const int ERR_BAD_VALUE = 205;
const int ERR_TIME_OVERFLOW = 101;
const int ERR_SERIAL_TIMEOUT = 102;


void sendErrorMessage(int value);
void sendExcMessage(int value);
void sendSetMessageResp(String setting, int value);
void sendSetMessageResp(String setting, int value, int extrValue);
void sendGetMessageResp(String get_message, int value);
void sendGetMessageResp(String get_message, int value, int extrValue);
void sendLogMessage(String message);
bool parseCommandParams(String data_str, String init_command, int results[], int arr_length);

#endif
