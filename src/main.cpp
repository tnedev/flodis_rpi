#include "Arduino.h"
#include "Wire.h"
#include "CoolingChamber.h"
#include "Bottle.h"
#include "Config.h"
#include "RPiComm.h"

unsigned long _lastTime;

String _data="";
boolean canWrite = false;

void setup(){

    pinMode(13, OUTPUT);
    CoolingChamber::init();
    Bottle::init();

    Serial.begin(115200);

}

void refreshTimer(){
    _lastTime = millis();
}

void startOperation(){
    CoolingChamber::start();
}

void stopOperation(){
    CoolingChamber::stop();
}
void timeoutCheck(){
    long timer = millis()-_lastTime;
    if(timer<0){
        sendErrorMessage(ERR_ARDUINO_READ_TIMEOUT);
    }
    else if (timer>SERIAL_TIMEOUT && canWrite){
        canWrite = false;
        _data = "";
        Serial.flush();
        sendErrorMessage(ERR_ARDUINO_READ_TIMEOUT);
    }
    else if(timer>SERIAL_REFRESH_TIMER){
        refreshTimer();
    }
}

void handleData(){
    int commands[10];

    if (_data.startsWith(HEARTBEAT_MSG)){
        // TODO: C
        if (CoolingChamber::isCooling() && CoolingChamber::getTempSensorData() == ERROR_TEMP){
            sendErrorMessage(ERR_TEMP_SENSOR);
        }
        // TODO: Add pressure problems as well.
        else {
            sendSetMessageResp(HEARTBEAT_MSG, HEARTBEAT_VAL);
        }
    }
    // Set Temp Target
    else if(_data.startsWith(SET_TEMP_TARGET_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, SET_TEMP_TARGET_MSG, commands, 10)) {
            sendErrorMessage(ERR_BAD_MESSAGE); // bad request
            return;
        }

        // the command was with a  valid structure
        int temp_target_int = commands[0];
        float  temp_target = temp_target_int/100.0;
        if (temp_target<= TEMP_TARGET_MIN || temp_target>= TEMP_TARGET_MAX) {
            sendErrorMessage(ERR_TEMP_TARGET); // Wrong value range
            return;
        }

        // if the value is fine, set the new value and send response
        CoolingChamber::setTempTarger(temp_target);
        sendSetMessageResp(SET_TEMP_TARGET_MSG, temp_target_int);
    }
    // get temperature
    else if (_data.startsWith(GET_TEMP_MSG)){
        float temp = CoolingChamber::getTempSensorData();
        if (temp == ERROR_TEMP){
            sendErrorMessage(ERR_TEMP_SENSOR);
        }
        else {
            sendGetMessageResp(GET_TEMP_MSG,int(temp*100));
        }
    }
    // get temperature target
    else if (_data.startsWith(GET_TEMP_TARGET_MSG)){
        sendGetMessageResp(GET_TEMP_TARGET_MSG,int(CoolingChamber::getTempTarget()*100.0));
    }

    // get the pressure
    else if(_data.startsWith(GET_PRESSURE_MSG)){
        int pressure = 1230; //TODO: implement getting the real pressure
        sendGetMessageResp(GET_PRESSURE_MSG, pressure);
    }
    // serve a drink
    else if(_data.startsWith(SERVE_DRINK_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, SERVE_DRINK_MSG, commands, 10)) {
            sendErrorMessage(ERR_BAD_MESSAGE); // bad request
            return;
        }

        int bottle = commands[0]; // get the value of the first element
        int servingTime = commands[1]; // get the value of the second element§
        if (bottle<=0 || bottle>BOTTLES){ // check the values
            sendErrorMessage(ERR_BOTTLE_NUMBER);
            return;
        }
        else if(servingTime<=0 || servingTime >=SERVING_TIME_MAX){
            sendErrorMessage(ERR_SERVING_TIME);
            return;
        }
        sendSetMessageResp(SERVE_DRINK_MSG, bottle, servingTime);
        Bottle::serve(bottle, servingTime);

    }
    // check for glass
    else if(_data.startsWith(CHECK_GLASS_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, CHECK_GLASS_MSG, commands, 10)) {
            sendErrorMessage(ERR_BAD_MESSAGE); // bad request
            return;
        }

        int bottle = commands[0]; // get the value of the first element
        if (bottle<=0 || bottle>BOTTLES){
            sendErrorMessage(ERR_BOTTLE_NUMBER);
            return;
        }

        int hasGlass = Bottle::checkForGlass(bottle);
        sendGetMessageResp(CHECK_GLASS_MSG, bottle, hasGlass);
    }
    else if (_data.startsWith(START_OPERATION_MSG)){
        sendSetMessageResp(START_OPERATION_MSG, 1);
        startOperation();
    }
    else if (_data.startsWith(STOP_OPERATION_MSG)){
        sendSetMessageResp(STOP_OPERATION_MSG, 1);
        stopOperation();
    }
    else if (_data.startsWith(START_COOLING_MSG)){
        sendSetMessageResp(START_COOLING_MSG, 1);
        CoolingChamber::start();
    }
    else if (_data.startsWith(STOP_COOLING_MSG)){
        sendSetMessageResp(STOP_COOLING_MSG, 1);
        CoolingChamber::stop();
    }
    // unrecognized initial/function command
    else {
        sendErrorMessage(ERR_BAD_MESSAGE);
    }

}

void serialEvent(){
    while(Serial.available()){
        char charData = Serial.read();
        timeoutCheck(); // check for timeout
        if( (charData == START_BYTE) && (_data=="") && !canWrite){
            canWrite = true;
            refreshTimer();
        }
        else if( (charData == START_BYTE) && (_data!="")){
            /*  If you trying to write to non-empty buffer, close
            *   the message and empty the buffer String. Send error
            *   message to the RPi
            */
            canWrite = false;
            _data = "";
            Serial.flush();
            sendErrorMessage(ERR_NON_EMPTY_BUFF);
        }
        else if( (charData == END_BYTE) && (_data!="") && canWrite){
            canWrite = false; // stop filling the data buffer when receive END_BYTE
            handleData();
            _data = ""; // empty the data buffer after handling it
        }
        else if( (charData == END_BYTE) && (_data=="") && canWrite){
            sendErrorMessage(ERR_CLOSE_EMPTY_BUFF);
        }

        else if( (charData != START_BYTE) && (!canWrite)){
            sendErrorMessage(ERR_NO_START_BYTE);
        }
        else if( (charData != START_BYTE) && (canWrite) ){
            _data+=String(charData);
        }

    }

}

void loop(){

    timeoutCheck(); // Clean message data if timeout

}
