#include <SD.h>
#include <SPI.h>

#define CHIP_SELECT_PIN 17
#define ODD_PIN_BEHAVIOUR 5

String dataString;

void setup() {
  Serial.begin(115200);

  pinMode(CHIP_SELECT_PIN, OUTPUT);
  pinMode(ODD_PIN_BEHAVIOUR, OUTPUT);

  while (!SD.begin(CHIP_SELECT_PIN)) {
    Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
    delay(1000);
  }
  File dataFile;
  dataFile = SD.open("/data.txt", FILE_WRITE);
  while (!dataFile) {
    Serial.println("FILE NOT OPEN");
    dataFile = SD.open("/data.txt", FILE_WRITE);
    delay(1000);
  }

  dataFile.close();
}

void loop() {
  // put your main code here, to run repeatedly:
  logData();
  delay(1000);
}

void logData() {
  dataString += "Current Time: ";
  dataString += millis();

  File dataFile;
  dataFile = SD.open("/data.txt", FILE_APPEND);
  if (!dataFile) {
    Serial.println("FAILED TO OPEN FILE TO WRITE");
  } else {
    dataFile.println(dataString);
    dataFile.close();

    Serial.print(dataString);
  }

  dataString = "";
}