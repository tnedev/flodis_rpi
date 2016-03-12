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
    Serial.println(set_message + String(extrValue) + SPLIT_BYTE + String(value));
}

void sendGetMessageResp(String get_message, int value){
    Serial.println(get_message + String(value));
}

void sendGetMessageResp(String get_message, int value, int extrValue){
    Serial.println(get_message+String(extrValue) + SPLIT_BYTE + String(value));
}

void sendLogMessage(String message){
    Serial.println("log:" + message);
}

void initParsedCommandArray(int results[], int arr_length) {
    for (int i =0; i <= arr_length - 1; i++) {
        results[i] = -1;
    }
}

bool isValidParsedCommandArray(int results[], int arr_length) {
    bool error = false;
    int i = 0;

    while (!error && i <= arr_length - 1) {
        error = results[i] != -1;
    }

    return !error;
}

bool parseCommandParams(String data_str, String init_command, int results[], int arr_length) {
    /* This function tokenizes the command string by using the & separator. It will
    * handle up to ten commands.If the structure of the string does not allow
    * such parsing, then all values will be -1. No missing values are allowed.
    * No & next to another & is allowed.
    *
    * The method returns false if there was an error and true if there was no error
    */

    String command = data_str.substring(init_command.length() + 1);
    initParsedCommandArray(results, arr_length);

    int i_res = 0;
    unsigned int j_comm = 0;
    unsigned int j_comm_start_reading = 0;
    boolean error = false;

    do {
        char curr_char = command.charAt(j_comm);

        if (curr_char != SPLIT_BYTE) {
            if (curr_char < '0' || curr_char > '9') {
                // if it is not simply a digit ... error
                error = true;
            }
            // if it is simply a digit ... keep reading
        } else {
            if (j_comm_start_reading == j_comm) {
                // a separator symbol without a value that is separates .. error
                error = true;
            } else {
                // we have reached an & and everything was good so far
                // take everything from the start reading index onwards
                String str = command.substring(j_comm_start_reading, j_comm);
                int curr_val = str.toInt();
                // assign to the array with the results
                results[i_res] = curr_val;
                // update the iter for the command string
                i_res = i_res + 1;
                // update the start reading index to indicate the expected beginning
                // of the next int
                j_comm_start_reading = j_comm + 1;
            }
        }

        //increment the command cursor
        j_comm = j_comm + 1;
    }   while (j_comm < command.length() && i_res < 10 && !error);

    if (error) {
        initParsedCommandArray(results, arr_length);
    }

    return error;
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

        if(data.charAt(i)==VALUES_BYTE && (i+1)<data.length() && !writeOne && !writeTwo){
            // check for the value start byte
            writeOne = true;
            writeTwo = false;
            i++;
        }
        else if(data.charAt(i)==VALUES_BYTE && (i+1)<data.length() && (writeOne || writeTwo)){
            // a second value start byt was faund -> fault message
            values[0] = "";
            values[1] = "";
            writeOne = false;
            i = data.length();
            break;
        }
        if(data.charAt(i)==SPLIT_BYTE && (i+1)<data.length() && !writeTwo && writeOne){
            // check for split byte meaning second value is comming
            writeOne = false;
            writeTwo = true;
            i++;
        }
        else if(data.charAt(i)==SPLIT_BYTE && (i+1)<data.length() && (writeTwo||writeOne)){
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
