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

const String heartbeatMessage = "hb:";
const String setTempTargetMessage = "stt:";
const String getTempTargetMessage = "gtt:";
const String getTempMessage = "gt:";
const String checkForGlassMessage = "cfg:";
const String getBottleQuantityMessage = "gbq:";
const String setBottleQuantityMessage = "sbq:";
const String getPressureMessage = "gp:";
const String serveDrinkMessage = "sd:";

void errorMessage(String message);
void excMessage(String message);
void setMessageResp(String setting, int value);
void setMessageResp(String setting, int value, int extrValue);
void getMessageResp(String get_message, int value);
void getMessageResp(String get_message, int value, int extrValue);
void errorBadRequest();
void logMessage(String message);

 #endif
