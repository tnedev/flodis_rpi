#include "RPiComm.h"

void sendErrorMessage(int value){
    Serial.println("Err:" + String(value));
}

void sendExcMessage(int value){
    Serial.println("Exc:" + String(value));
}

void sendSetMessageResp(String set_message, int value){
    Serial.println(set_message+String(value));
}

void sendSetMessageResp(String set_message, int value, int extrValue){
    Serial.println(set_message + String(extrValue) + splitByte + String(value));
}

void sendGetMessageResp(String get_message, int value){
    Serial.println(get_message + String(value));
}

void sendGetMessageResp(String get_message, int value, int extrValue){
    Serial.println(get_message+String(extrValue) + splitByte + String(value));
}

void sendLogMessage(String message){
    Serial.println("log:" + message);
}

int * splitData(String data){
    /* This function splits the data and returns a pointer to a int array with two values
    *
    * return:
    *  value[0] = int, value[1] = int -> all good
    *  value[0] or value[1] = -1 -> the -1 value was missing
    *  value[0] and value[1] = -1 -> bad message request
    */

    String values[2]; // storage strings
    boolean writeOne=false; // true if we are filling value[0]
    boolean writeTwo=false; // true if we are filling value[1]

    for (unsigned int i = 0; i < data.length(); i++) {

        if(data.charAt(i)==valuesByte && (i+1)<data.length() && !writeOne && !writeTwo){
            // check for the value start byte
            writeOne = true;
            writeTwo = false;
            i++;
        }
        else if(data.charAt(i)==valuesByte && (i+1)<data.length() && (writeOne || writeTwo)){
            // a second value start byt was faund -> fault message
            values[0] = "";
            values[1] = "";
            writeOne = false;
            i = data.length();
            break;
        }
        if(data.charAt(i)==splitByte && (i+1)<data.length() && !writeTwo && writeOne){
            // check for split byte meaning second value is comming
            writeOne = false;
            writeTwo = true;
            i++;
        }
        else if(data.charAt(i)==splitByte && (i+1)<data.length() && (writeTwo||writeOne)){
            // we fould a split byte, but value one is still filling -> fault message
            values[0] = "";
            values[1] = "";
            writeTwo = false;
            i = data.length();
            break;
        }
        if(writeOne){
            // writing to value[0]
            values[0]+=String(data.charAt(i));
            if (data.charAt(i)<48 || data.charAt(i)>58){ //check if the char is a digit
                values[0]=""; //clear the string if other value was faund -> fault message
                values[1]="";
                i = data.length();
                break; // break the loop
            }
        }
        else if(writeTwo){
            // writing to value[1]
            values[1]+=String(data.charAt(i));
            if (data.charAt(i)<48 || data.charAt(i)>58){ //check if the char is a digit
                values[1]=""; //clear the string if other value was faund -> fault message
                values[0]="";
                i = data.length();
                break; // break the loop
            }
        }
    }
    static int intData[2];
    if(values[0]==""){
        // if the message was empty return -1, otherwise cast to int
        intData[0]=-1;
    }
    else {
        intData[0] = values[0].toInt();
    }
    if(values[1]==""){
        // if the message was empty return -1, otherwise cast to int
        intData[1]=-1;
    }
    else {
        intData[1] = values[1].toInt();
    }

    return intData; // return *int
}
