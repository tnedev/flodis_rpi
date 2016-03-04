
#include "Arduino.h"
#include "Wire.h"
#include "Commander.h"
#include "CoolingChamber.h"

#define SLAVE_ADDRESS 0x04

static boolean state=true;
byte Commander::_data[32];
byte Commander::_dataSize=0;

Commander::Commander(){
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void Commander::receiveData(int byteCount){
  _dataSize = byteCount;
  int i = 0;
  while(Wire.available()){
    _data[i] = Wire.read();
  }
}

void Commander::sendData(){
  switch (_data[0]) {
    case 1: // get current temerature
      Commander::sendTemp();
    case 2:
      digitalWrite(13, state);
      state = !state;

  }

}

void Commander::sendTemp(){
  char buffer4[4];
  char buffer5[5];
  int temp = CoolingChamber::getTemp();
  if (temp<10.00f && temp>0.00f){
    dtostrf(temp,4,2,buffer4);
    Wire.write(buffer4);
  }
  else{
    dtostrf(temp,4,2,buffer5);
    Wire.write(buffer5);
  }

}
