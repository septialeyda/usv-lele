#include <SoftwareSerial.h>
#include "SoftwareSerialRS232.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_7segment redDisplay = Adafruit_7segment();
Adafruit_7segment blueDisplay = Adafruit_7segment();

SoftwareSerialRS232 scaleRed;
SoftwareSerialRS232 scaleBlue;

SoftwareSerial redScale(10,9);  //RX pin 10
SoftwareSerial blueScale(8,7);  //RX pin 8 

boolean serialDebug = false;

int weight;

void setup() {
  if (serialDebug) { Serial.begin(9600); }
  redScale.begin(9600);
  blueScale.begin(9600);
  scaleRed.init(&redScale);
  scaleBlue.init(&blueScale);
  redDisplay.begin(0x70);
  blueDisplay.begin(0x71);
}

void loop() { 
  scaleBlue.listen();
  scaleBlue.readString();
  weight = scaleBlue.parseData();
  blueDisplay.print(weight, DEC);
  blueDisplay.writeDisplay();
  if (serialDebug) {
    Serial.print("Blue: ");
    Serial.print(weight); 
  }

  scaleRed.listen();
  scaleRed.readString();
  weight = scaleRed.parseData();
  redDisplay.print(weight, DEC);
  redDisplay.writeDisplay();
  if (serialDebug) {
    Serial.print("  Red: ");
    Serial.println(weight); 
  }
}
