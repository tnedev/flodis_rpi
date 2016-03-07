#include "Arduino.h"
#include "CoolingChamber.h"
#include "Bottle.h"
#include "Wire.h"

#define SLAVE_ADDRESS 0x04
// I2C Slave Address of the Arduino. Used for communication with the RPi

byte _data[32];
byte _dataSize=0;

void sendTemp(){
  // The RPi asked for temperature data.
  // sendTemp() asks the CoolingChamber for the temperature ans send it to the RPi
  // Sends float as a string

    char buffer4[4]; // 4 character buffer
    char buffer5[5]; // 5 character buffer
    int temp = CoolingChamber::getTemp();
    if (temp<10.00f && temp>0.00f){
      // If the temperature is between 0.00 and 9.99 send the data as 4 characters
      dtostrf(temp,4,2,buffer4);
      Wire.write(buffer4);
    }
    else{
      // If the temperature is >=10.00 or <0.00 send the data as 5 characters
      dtostrf(temp,4,2,buffer5);
      Wire.write(buffer5);
    }

}

void receiveData(int byteCount){
  _dataSize = byteCount;
  int i = 0;
  while(Wire.available()){
    _data[i] = Wire.read();
  }
}

void reply(){
  switch (_data[0]) {
    case 0:
      Wire.write("1");
    case 1: // get current temerature
      sendTemp();
    case 2:
    //  digitalWrite(13, state);
    //  state = !state;
    ;

  }

}

void setup(){

  CoolingChamber::init(13,2,5.00f, 0.5f);
  CoolingChamber::start(); // Start the cooling process

  Wire.begin(SLAVE_ADDRESS); // Start the I2C communication
  Wire.onReceive(receiveData); // Callback for receiving data through the i2c
  Wire.onRequest(reply); // Callback for replying with data through the i2c.

}

void loop(){
  //testing2 dsf

}
