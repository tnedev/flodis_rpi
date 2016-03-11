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
        sendErrorMessage(errTimerOverflow);
    }
    else if (timer>serialTimeout && canWrite){
        canWrite = false;
        _data = "";
        Serial.flush();
        sendErrorMessage(errSerialTimeout);
    }
    else if(timer>30000){
        refreshTimer();
    }
}

void handleData(){
    if (_data.startsWith(heartbeatMessage)){
        sendSetMessageResp(heartbeatMessage,1);
    }
    // Set Temp Target
    else if(_data.startsWith(setTempTargetMessage)){
        int temp_target_int = *splitData(_data);
        float temp_target;
        if (temp_target_int>=0){ // check if the message was coorect
            temp_target = float(temp_target_int)/100.0; // convert to float
            if(temp_target>0.0f && temp_target<40.0f){ // check if the value is within range
                CoolingChamber::setTempTarger(temp_target); // if the value is fine, set the new value and send response
                sendSetMessageResp(setTempTargetMessage, int (temp_target*100));
            }
            else{
                sendErrorMessage(errBadValue); // Wrong value range
            }
        }
        else {
            sendErrorMessage(errBadRequestValue); // bad request
        }
    }
    // get temperature
    else if (_data.startsWith(getTempMessage)){
        int temp = int(CoolingChamber::getTempSensorData()*100);
        sendGetMessageResp(getTempMessage,temp);
    }
    else if (_data.startsWith(getTempTargetMessage)){
        sendGetMessageResp(getTempTargetMessage,int(CoolingChamber::getTempTarget()*100.0));
    }
    // get quantity of a bottle
    else if(_data.startsWith(getBottleQuantityMessage)){
        int bottle = *splitData(_data);
        int bottle_quantity = 200; //TODO: implement getting the quantity
        if (bottle<=0 || bottle>bottles){ // check for correct message
            sendErrorMessage(errBadRequestValue);
        }
        else {
            sendGetMessageResp(getBottleQuantityMessage, bottle_quantity, bottle);
        }
    }
    // set the quantity of a bottle
    else if(_data.startsWith(setBottleQuantityMessage)){
        int *data = splitData(_data); // returns the pointer to int array
        int bottle = *data; // get the value of the first element
        int quantity = *(data+1); // get the value of the second element
        Serial.println(bottle);
        Serial.println(quantity);
        if (bottle<=0 || bottle>bottles || quantity<0){ // check if values are fine
            sendErrorMessage(errBadRequestValue);
        }
        else if (quantity>2000) { // cant sent this much quantity
            sendErrorMessage(errBadValue);
        }
        else{
            sendSetMessageResp(setBottleQuantityMessage, quantity, bottle);
        }
    }
    // get the pressure
    else if(_data.startsWith(getPressureMessage)){
        int pressure = 1230; //TODO: implement getting the real pressure
        sendGetMessageResp(getPressureMessage, pressure);
    }
    // serve a drink
    else if(_data.startsWith(serveDrinkMessage)){
        int *data = splitData(_data);
        int bottle = *data;
        int quantity = *(data+1);
        if (bottle<=0 || bottle>bottles || quantity<=0 || quantity>2000){ // check the values
            sendErrorMessage(errBadRequestValue);
        }
        else {
            // TODO: Call serving operation, if successfull send response, otherwise error
            sendSetMessageResp(serveDrinkMessage, quantity, bottle);
        }
    }
    // check for glass
    else if(_data.startsWith(checkForGlassMessage)){
        int bottle = *splitData(_data);
        if (bottle<=0 || bottle>bottles){
            sendErrorMessage(errBadRequestValue);
        }
        else {
            int hasGlass = 0; //TODO: check for glass
            sendGetMessageResp(checkForGlassMessage, hasGlass, bottle);
        }
    }
    else {
        sendErrorMessage(errBadRequestValue);
    }

}

void serialEvent(){
    while(Serial.available()){
        char charData = Serial.read();
        timeoutCheck(); // check for timeout
        if( (charData == startByte) && (_data=="") && !canWrite){
            canWrite = true;
            refreshTimer();
        }
        else if( (charData == startByte) && (_data!="")){
            /*  If you trying to write to non-empty buffer, close
            *   the message and empty the buffer String. Send error
            *   message to the RPi
            */
            canWrite = false;
            _data = "";
            Serial.flush();
            sendErrorMessage(errNonEmptyBuffer);
        }
        else if( (charData == endByte) && (_data!="") && canWrite){
            canWrite = false; // stop filling the data buffer when receive endByte
            handleData();
            _data = ""; // empty the data buffer after handling it
        }
        else if( (charData == endByte) && (_data=="") && canWrite){
            sendErrorMessage(errCloseEmptyBuffer);
        }

        else if( (charData != startByte) && (!canWrite)){
            sendErrorMessage(errNoStartByte);
        }
        else if( (charData != startByte) && (canWrite) ){
            _data+=String(charData);
        }

    }

}

void loop(){

    timeoutCheck(); // Clean message data if timeout

}
