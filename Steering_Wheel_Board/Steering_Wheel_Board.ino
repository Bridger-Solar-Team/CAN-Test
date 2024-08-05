#include <CAN.h>
#include <SD.h>
#include <SPI.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define RIGHT_TURN_PIN 8
#define LEFT_TURN_PIN 9
#define CHIP_SELECT_PIN 10
#define STEERING_CAN_ID 0x12
#define CONTROL_CAN_ID 0x11 //must be less than 0x800 (2^11 in hex)
#define THROTTLE_PIN 11

int accel;
bool leftTurn;
bool rightTurn;
uint8_t canData[8];
File dataFile;
String dataString;
byte controlData[8];

void setup() {
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3);
  CAN.onReceive(readCAN);

  SD.begin();
  dataFile = SD.open("data.txt", FILE_WRITE);

  pinMode(RIGHT_TURN_PIN, INPUT);
  pinMode(LEFT_TURN_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(CHIP_SELECT_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  writeCAN();
  readInputs();
  logData();
}

void logData() {
  dataString += leftTurn;
  dataString += rightTurn;
  dataString += ",";
  dataString += 
  dataFile.println(dataString);
  dataString = "";
}

void readInputs() {
  leftTurn = digitalRead(LEFT_TURN_PIN);
  rightTurn = digitalRead(RIGHT_TURN_PIN);
  accel = analogRead(THROTTLE_PIN)/4095.0;
}

void readCAN(int packetSize) {
  int dataID = CAN.packetId();
  int position = 0;
  if(dataID == CONTROL_CAN_ID) {
    while(CAN.available()) {
      controlData[position] = CAN.read();
      position += 1;
    }
  }
}

void writeCAN() {
  canData[0] += leftTurn;
  canData[0] += rightTurn << 1;
  canData[1] = accel*255;

  CAN.beginPacket(STEERING_CAN_ID);
  CAN.write(canData,8);
  CAN.endPacket();

}