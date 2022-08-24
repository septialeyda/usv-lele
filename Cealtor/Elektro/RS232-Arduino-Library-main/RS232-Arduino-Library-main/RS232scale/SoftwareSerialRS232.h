#ifndef SOFTWARESERIALRS232_H
#define SOFTWARESERIALRS232_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class SoftwareSerialRS232 {
  
  private:
    SoftwareSerial *_serialConnection;
    byte STX = 0x02;  //Start Code
    byte CR = 0x0d;   //End Code
    boolean receivedString; 
    boolean foundStart;
    String Data;
    
  public:
    SoftwareSerialRS232();
    void init(SoftwareSerial *_serial);
    bool listen();
    void readString();
    int parseData();
    virtual int available();
    virtual int read();
};

#endif
