#include <SD.h>
#include <SPI.h>

#define CHIP_SELECT_PIN 10

File dataFile;
String dataString;

void setup() {

  SD.begin();
  dataFile = SD.open("data.txt", FILE_WRITE);

  pinMode(CHIP_SELECT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  logData();
}

void logData() {
  dataString += "Current Time: ";
  dataString += millis();
  dataFile.println(dataString);
  dataString = "";
}