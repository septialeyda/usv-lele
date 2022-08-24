#include "SoftwareSerialRS232.h"

SoftwareSerialRS232::SoftwareSerialRS232() {
  _serialConnection = NULL;
}

void SoftwareSerialRS232::init(SoftwareSerial *_serial) {
  _serialConnection = _serial;
}

bool SoftwareSerialRS232::listen() {
  _serialConnection->listen();
}

int SoftwareSerialRS232::available() {
  _serialConnection->available();
}

int SoftwareSerialRS232::read() {
  _serialConnection->read();
}

void SoftwareSerialRS232::readString() {
  receivedString = false;
  foundStart = false;
  Data = "";
  while(!receivedString) {
    if (this->available()) {
      char character = this->read();    
      if(character == STX) { foundStart = true; }
      else if(foundStart && character == CR) { 
          receivedString = true; 
      }
      else if(foundStart) { Data.concat(character); }
    }
  }
}

  // Red Scale Format:  <STX><POL><wwwwwww><UNIT><G/N><S><CR>
  // <POL> = space or "-"
  // <wwwwwww>  right justified, leading zeros as spaces
  // <UNIT> = L for lbs
  // <G/N> = G for gross
  // <S> = space for valid data
  // '      98LG ' or '-     98LG '

  // Blue Scale Format: <STX><Signed Data><sp><lb/kg><CR>
  // <Signed Data> is 8 characters, 1st is either minus or plus, padded with leading spaces
  // '     +98 lb'  or '     -98 lb'

  // Note:  toInt() works from the left and stops with a non-number... so 98LG --> 98 and 98 lb --> 98
  
int SoftwareSerialRS232::parseData() {
  // both formats give an 11 character string:  return an unrealistic weight to show an error
  if(Data.length() != 11) { return -10000; }  
  
  // received correct length string
  else {    

      // Red negative weight
      if( Data.charAt(0) == '-' ) {
        Data = Data.substring(1, Data.length());  //remove negative sign
        Data.trim(); // remove leading spaces
        return Data.toInt() * -1; 
      }
      
      
      else {
        Data.trim();  // remove leading spaces
        
        // Blue positive weight
        if(Data.charAt(0) == '+' ) { 
            Data = Data.substring(1, Data.length()); //remove positive sign
            return Data.toInt(); 
            }
        
        // Blue negative weight
        else if(Data.charAt(0) == '-') {
            Data = Data.substring(1, Data.length()); //remove negative sign
            return Data.toInt() * -1; 
        }
        
        //Red positive weight
        else return Data.toInt();
      }
   }
}
