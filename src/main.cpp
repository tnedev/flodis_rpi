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


void handleData(){
  if (_data.startsWith(heartbeatMessage)){
    getMessageResp(heartbeatMessage, 1);
  }
  else if(_data.startsWith(setTempTargetMessage)){
    float temp_target = _data.substring(setTempTargetMessage.length()).toFloat()/100.0f;
    CoolingChamber::setTempTarger(temp_target);
    setMessageResp(setTempTargetMessage, int (temp_target*100));
  }
  else if (_data.startsWith(getTempMessage)){
    int temp = int(CoolingChamber::getTempSensorData()*100);
    getMessageResp(getTempMessage,temp);
  }
  else if(_data.startsWith(getBottleQuantityMessage)){
    int bottle = _data.substring(getBottleQuantityMessage.length()).toInt();
    int bottle_quantity = 200; //TODO: implement getting the quantity
    if (bottle<=0 || bottle>bottles){
      errorBadRequest();
    }
    else {
      getMessageResp(getBottleQuantityMessage, bottle_quantity, bottle);
    }
  }
  else if(_data.startsWith(setBottleQuantityMessage)){
    int bottle = _data.substring(getBottleQuantityMessage.length(), getBottleQuantityMessage.length()+1).toInt();
    if (bottle<=0 || bottle>bottles){
      errorBadRequest();
    }
    else {
      int quantity = _data.substring(getBottleQuantityMessage.length()+2).toInt();
      setMessageResp(setBottleQuantityMessage, quantity, bottle);
    }
  }
  else if(_data.startsWith(getPressureMessage)){
    int pressure = 1230; //TODO: implement getting the real pressure
    getMessageResp(getPressureMessage, pressure);
  }
  else if(_data.startsWith(serveDrinkMessage)){
    int bottle = _data.substring(serveDrinkMessage.length(), serveDrinkMessage.length()+1).toInt();
    if (bottle<=0 || bottle>bottles){
      errorBadRequest();
    }
    else {
      int quantity = _data.substring(serveDrinkMessage.length()+2).toInt();
      // TODO: Call serving operation, if successfull send response, otherwise error
      setMessageResp(serveDrinkMessage, quantity, bottle);
    }
  }
  else if(_data.startsWith(checkForGlassMessage)){
    int bottle = _data.substring(checkForGlassMessage.length()).toInt();
    if (bottle<=0 || bottle>bottles){
      errorBadRequest();
    }
    else {
      int hasGlass = 0; //TODO: check for glass
      getMessageResp(checkForGlassMessage, hasGlass, bottle);
    }
  }
  else {
    errorBadRequest();
  }

}

void refreshTimer(){
  _lastTime = millis();
}

void timeoutCheck(){
  long timer = millis()-_lastTime;
  if(timer<0){
    errorMessage("Timer overflow");
  }
  else if (timer>serialTimeout && canWrite){
    canWrite = false;
    _data = "";
    Serial.flush();
    errorMessage("Serial timeout");
  }
  else if(timer>30000){
    refreshTimer();
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
          errorMessage("Can't start on non-empty buffer");
      }
      else if( (charData == endByte) && (_data!="") && canWrite){
          canWrite = false; // stop filling the data buffer when receive endByte
          handleData();
          _data = ""; // empty the data buffer after handling it
      }
      else if( (charData == endByte) && (_data=="") && canWrite){
        errorMessage("Can't close empty buffer");
      }

      else if( (charData != startByte) && (!canWrite)){
        errorMessage("Can't write to buffer before start byte");
      }
      else if( (charData != startByte) && (canWrite) ){
          _data+=String(charData);
      }

  }

}

void loop(){

  timeoutCheck(); // Clean message data if timeout

}
