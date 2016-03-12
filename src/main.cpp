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
    //CoolingChamber::start(); // Start the cooling process

    Serial.begin(115200);

}

void refreshTimer(){
    _lastTime = millis();
}

void timeoutCheck(){
    long timer = millis()-_lastTime;
    if(timer<0){
        sendErrorMessage(ERR_TIME_OVERFLOW);
    }
    else if (timer>SERIAL_TIMEOUT && canWrite){
        canWrite = false;
        _data = "";
        Serial.flush();
        sendErrorMessage(ERR_SERIAL_TIMEOUT);
    }
    else if(timer>SERIAL_REFRESH_TIMER){
        refreshTimer();
    }
}

void handleData(){
    int commands[10];

    if (_data.startsWith(HEARTBEAT_MSG)){
        sendSetMessageResp(HEARTBEAT_MSG, HEARTBEAT_VAL);
    }
    // Set Temp Target
    else if(_data.startsWith(SET_TEMP_TARGET_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, SET_TEMP_TARGET_MSG, commands, 10)) {
             sendErrorMessage(ERR_BAD_REQUEST_VAL); // bad request
             return;
        }

        // the command was with a  valid structure
        int temp_target = commands[0];
        if (temp_target <= 0) {
            sendErrorMessage(ERR_BAD_REQUEST_VAL); // bad request
            return;
        }

        if (temp_target<0 && temp_target>4000) {
            sendErrorMessage(ERR_BAD_VALUE); // Wrong value range
            return;
        }

        // if the value is fine, set the new value and send response
        CoolingChamber::setTempTarger(temp_target/100.0);
        sendSetMessageResp(SET_TEMP_TARGET_MSG, int (temp_target));
    }
    // get temperature
    else if (_data.startsWith(GET_TEMP_MSG)){
        int temp = int(CoolingChamber::getTempSensorData()*100);
        sendGetMessageResp(GET_TEMP_MSG,temp);
    }
    // get temperature target
    else if (_data.startsWith(GET_TEMP_TARGET_MSG)){
        sendGetMessageResp(GET_TEMP_TARGET_MSG,int(CoolingChamber::getTempTarget()*100.0));
    }
    // get quantity of a bottle
    else if(_data.startsWith(GET_BOTTLE_QUANT_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, GET_BOTTLE_QUANT_MSG, commands, 10)) {
             sendErrorMessage(ERR_BAD_REQUEST_VAL); // bad request
             return;
        }

        int bottle = commands[0];
        if (bottle<=0 || bottle>BOTTLES){ // check for correct message
            sendErrorMessage(ERR_BAD_REQUEST_VAL);
            return;
        }

        int bottle_quantity = 200; //TODO: implement getting the quantity
        sendGetMessageResp(GET_BOTTLE_QUANT_MSG, bottle_quantity, bottle);
    }
    // set the quantity of a bottle
    else if(_data.startsWith(SET_BOTTLE_QUANT_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, SET_BOTTLE_QUANT_MSG, commands, 10)) {
             sendErrorMessage(ERR_BAD_REQUEST_VAL); // bad request
             return;
        }

        int bottle = commands[0]; // get the value of the first element
        int quantity = commands[1]; // get the value of the second element§
        Serial.println(bottle);
        Serial.println(quantity);

        if (bottle<=0 || bottle>BOTTLES || quantity<0){ // check if values are fine
            sendErrorMessage(ERR_BAD_REQUEST_VAL);
            return;
        }

        if (quantity>2000) { // cant sent this much quantity
            sendErrorMessage(ERR_BAD_VALUE);
            return;
        }

        sendSetMessageResp(SET_BOTTLE_QUANT_MSG, quantity, bottle);
    }
    // get the pressure
    else if(_data.startsWith(GET_PRESSURE_MSG)){
        int pressure = 1230; //TODO: implement getting the real pressure
        sendGetMessageResp(GET_PRESSURE_MSG, pressure);
    }
    // serve a drink
    else if(_data.startsWith(PREV_DRINK_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, PREV_DRINK_MSG, commands, 10)) {
             sendErrorMessage(ERR_BAD_REQUEST_VAL); // bad request
             return;
        }

        int bottle = commands[0]; // get the value of the first element
        int quantity = commands[1]; // get the value of the second element§
        if (bottle<=0 || bottle>BOTTLES || quantity<=0){ // check the values
            sendErrorMessage(ERR_BAD_REQUEST_VAL);
            return;
        }

        if (quantity>2000) { // cant sent this much quantity
            sendErrorMessage(ERR_BAD_VALUE);
            return;
        }

        sendSetMessageResp(PREV_DRINK_MSG, quantity, bottle);
    }
    // check for glass
    else if(_data.startsWith(CHECK_GLASS_MSG)){
        // parse and validate based on integers between &
        if (!parseCommandParams(_data, PREV_DRINK_MSG, commands, 10)) {
             sendErrorMessage(ERR_BAD_REQUEST_VAL); // bad request
             return;
        }

        int bottle = commands[0]; // get the value of the first element
        if (bottle<=0 || bottle>BOTTLES){
            sendErrorMessage(ERR_BAD_REQUEST_VAL);
            return;
        }

        int hasGlass = 0; //TODO: check for glass
        sendGetMessageResp(CHECK_GLASS_MSG, hasGlass, bottle);
    }
    // unrecognized initial/function command
    else {
        sendErrorMessage(ERR_BAD_REQUEST_VAL);
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
