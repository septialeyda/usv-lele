#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "Timer.h"

String msg_split[4];
String serial_buff, header, s_suhu, s_ph;

SoftwareSerial mySerial(10, 11); // RX, TX
LiquidCrystal_I2C lcd(0x3F, 16, 2);
Timer acc_timer;

void setup() {
  Serial.begin(9600);

  mySerial.begin(9600);
  lcd.init();
  lcd.backlight();
  Wire.begin();
  lcd.clear();
}

void loop() { // run over and over
  terimadata();
  tulis();
  if (acc_timer.elapsed(1000)) {
    lcd.clear();
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}

void split(String objectm, char delimiter) {
  byte i = 0;
  String temp = "";
  int commaPosition;
  do {
    commaPosition = objectm.indexOf(delimiter);
    if (commaPosition != -1) {
      temp = objectm.substring(0, commaPosition);
      objectm = objectm.substring(commaPosition + 1, objectm.length());
    }
    else {
      if (objectm.length() > 0) {
        temp = objectm;
      }
    }
    msg_split[i] = temp;
    i++;
  }
  while (commaPosition >= 0);
}

void terimadata() {
  if (mySerial.available()) {
    serial_buff = mySerial.readStringUntil('\n');
    //Serial.print(serial_buff);
    split(serial_buff, ',');

    header = msg_split[0];
    s_suhu = msg_split[1];
    s_ph = msg_split[2];
    Serial.println(header);
    Serial.print("Suhu:");
    Serial.println(s_suhu);
    Serial.print("pH:");
    Serial.println(s_ph);
  }
}

void tulis() {
  lcd.setCursor(0, 0);
  lcd.print("Suhu :");
  lcd.setCursor(7, 1);
  lcd.print(s_suhu);
  lcd.setCursor(0, 1);
  lcd.print("pH   :");
  lcd.setCursor(7, 0);
  lcd.print(s_ph);
  lcd.setCursor(12, 1);
  lcd.print(" ");
}
