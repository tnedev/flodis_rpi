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
    * handle up to arr_length commands. If the structure of the string does not allow
    * such parsing, then all values will be -1. No missing values are allowed.
    * No & next to another & is allowed.
    *
    * The method returns false if there was an error and true if there was no error
    */

    String command = data_str.substring(init_command.length());
    // inits everything in the awway to -1
    initParsedCommandArray(results, arr_length);

    int i_res = 0;
    unsigned int j_comm = 0;
    unsigned int j_comm_start_reading = 0;
    boolean error = false;

    while (j_comm < command.length() && i_res < arr_length && !error) {
        char curr_char = command.charAt(j_comm);

        if (curr_char != SPLIT_BYTE) {
            if (curr_char < '0' || curr_char > '9') {
                // if it is not simply a digit ... error
                error = true;
            }
            // if it is simply a digit ... keep reading
        } else {
            // this is a split & here
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

        //increment the command iterator
        j_comm = j_comm + 1;
    }

    if (!error) {
        // everything was OK up to now however...

        if (j_comm_start_reading == j_comm || i_res == arr_length) {
            // we ended with a split & not followed by int or we reached the end of
            // our command buffer
            error = true;
        } else {
            // there is an end integer that needs to be taken care of
            String str = command.substring(j_comm_start_reading, j_comm);
            int curr_val = str.toInt();
            // assign to the array with the results
            results[i_res] = curr_val;
        }
    }

    if (error) {
        initParsedCommandArray(results, arr_length);
    }

    // if there is no error (i.e. error is false) we want to return true for
    // "success"; vice-versa ... if there is an error, return retrun false for
    // "failure"
    return !error;
}
