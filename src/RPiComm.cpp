#include "RPiComm.h"

void errorMessage(String message){
  Serial.println("Err:" + message);
}

void excMessage(String message){
  Serial.println("Exc:" + message);
}

void setMessageResp(String set_message, int value){
  Serial.println(set_message+String(value));
}

void setMessageResp(String set_message, int value, int extrValue){
  Serial.println(set_message + String(extrValue) + splitByte + String(value));
}

void getMessageResp(String get_message, int value){
  Serial.println(get_message + String(value));
}

void getMessageResp(String get_message, int value, int extrValue){
  Serial.println(get_message+String(extrValue) + splitByte + String(value));
}

void errorBadRequest(){
  errorMessage("Bad Request");
}

void logMessage(String message){
  Serial.println("log:" + message);
}
