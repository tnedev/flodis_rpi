/* MessageQ.h is a simple message exchange system
 * Written by Tihomir Nedev in Feb 2016
 */

#ifndef Commander_h
#define Commander_h

#include "Arduino.h"
#include "Wire.h"

class Commander{
  public:
    Commander();
    static void sendData();
    static void receiveData(int byteCount);

  private:
    static void sendTemp();
    static byte _data[32];
    static byte _dataSize;

};

#endif
