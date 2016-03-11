/* Config.h is an configuration for all Arduino pins and communication commands,
* Use it to set the pin numbers or define a new command.
*/

#ifndef RPiComm_h
#define RPiCom_h

#include "Arduino.h"


const int serialTimeout = 250; //timeout before cancel the message [ms]

const char startByte = '[';
const char endByte = ']';
const char splitByte = '&';
const char valuesByte = ':';

const String heartbeatMessage = "hb:";
const String setTempTargetMessage = "stt:";
const String getTempTargetMessage = "gtt:";
const String getTempMessage = "gt:";
const String checkForGlassMessage = "cfg:";
const String getBottleQuantityMessage = "gbq:";
const String setBottleQuantityMessage = "sbq:";
const String getPressureMessage = "gp:";
const String serveDrinkMessage = "sd:";

const int errBadRequestValue = 201;
const int errNonEmptyBuffer = 202; //Can't start on non-empty buffer
const int errCloseEmptyBuffer = 203; //Can't close empty buffer
const int errNoStartByte = 204; //Can't write to buffer before start byte
const int errBadValue = 205;
const int errTimerOverflow = 101;
const int errSerialTimeout = 102;


void sendErrorMessage(int value);
void sendExcMessage(int value);
void sendSetMessageResp(String setting, int value);
void sendSetMessageResp(String setting, int value, int extrValue);
void sendGetMessageResp(String get_message, int value);
void sendGetMessageResp(String get_message, int value, int extrValue);
void sendLogMessage(String message);
int *splitData(String data);

#endif
