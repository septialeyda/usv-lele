#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment redDisplay = Adafruit_7segment();
Adafruit_7segment blueDisplay = Adafruit_7segment();

SoftwareSerial redScale(10,9);  //RX pin 10
SoftwareSerial blueScale(8,7);  //RX pin 8 
byte STX = 0x02;  //Start Code
byte CR = 0x0d;   //End Code
boolean receivedString = false; 
boolean foundStart = false;
String Data = "";
boolean serialDebug = false;

void setup() {
  if (serialDebug) { Serial.begin(9600); }
  redScale.begin(9600);
  blueScale.begin(9600);
  redDisplay.begin(0x70);
  blueDisplay.begin(0x71);
}

void loop() { 
  
  resetVariables();
  blueScale.listen();
  while(!receivedString) {
    if (blueScale.available()) {
      char character = blueScale.read();    
      if(character == STX) { foundStart = true; }
      else if(foundStart && character == CR) { 
          receivedString = true; 
          int weight = parseData(9);
          blueDisplay.print(weight, DEC);
          blueDisplay.writeDisplay();
          if (serialDebug) {
            Serial.print("Blue: ");
            Serial.print(weight);
          }
      }
      else if(foundStart) { Data.concat(character); }
    }
  }

  resetVariables();
  redScale.listen();
  while(!receivedString) {
    if(redScale.available()) {
      char character = redScale.read();    
      if(character == STX) { foundStart = true; }
      else if(foundStart && character == CR) { 
          receivedString = true; 
          int weight = parseData(8);
          redDisplay.print(weight, DEC);
          redDisplay.writeDisplay();
          if (serialDebug) {
            Serial.print("  Red: ");
            Serial.println(weight);
          }
      }
      else if(foundStart) { Data.concat(character); }
    }
  }
}

int parseData(byte splitEnd) {
  if(Data.length() != 11) { return -10000; }  // deal with this later... returning a unrealistic weight to show an error
  // Red Scale Format:  '    98LGZ' or '-    98LGZ'
  else {    
      if( Data.charAt(0) == '-' ) {
        Data = Data.substring(1, Data.length());
        Data.trim();
        return Data.toInt() * -1; 
      }
      else {
        Data.trim();
        if(Data.charAt(0) == '+' ) { 
            Data = Data.substring(1, Data.length());
            return Data.toInt(); 
            }
        else if(Data.charAt(0) == '-') {
            Data = Data.substring(1, Data.length());
            return Data.toInt() * -1; 
        }
        else return Data.toInt();
      }
   }
}

void resetVariables() {
  receivedString = false;
  Data = "";
  foundStart = false;
}

//Note:  Streamline code by creating a Serial class with a function that reads the RS232 data into a string?
//See https://stackoverflow.com/questions/7455570/how-to-pass-serial-object-by-reference-to-my-class-in-arduino
