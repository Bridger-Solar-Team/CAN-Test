//API reference: https://github.com/sandeepmistry/arduino-CAN/blob/master/API.md

#include <CAN.h>
#include "Constants.h"

bool leftTurn;
bool rightTurn;
bool mainPower;
float throttle;
float ccl;
float dcl;
bool fault;
bool estop;
bool brake;
bool cruiseControl;
bool hazards;

byte steeringData[8];
byte canData[8];

void setup() {
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3);
  CAN.onReceive(readCAN);
}

void loop() {
  readInputs();
  writeCAN();
}

void readInputs() {
  leftTurn = steeringData[0] & 0x01;
  rightTurn = steeringData[0] & 0x02;
  mainPower = steeringData[0] & 0x04;
  throttle = steeringData[1]/255.0;
  ccl = analogRead(CCL_PIN)/4095.0;
  dcl = analogRead(DCL_PIN)/4095.0;
  fault = digitalRead(FAULT_PIN);
  estop = digitalRead(ESTOP_PIN);
  brake = steeringData[0] & 0x08;
  cruiseControl = steeringData[0] & 0x10;
  hazards = steeringData[0] & 0x20;
}

void readCAN(int packetSize) {
  int dataID = CAN.packetId();
  int position = 0;
  if(dataID == STEERING_CAN_ID) {
    while(CAN.available()) {
      steeringData[position] = CAN.read();
      position += 1;
    }
  }
}

void writeCAN() {
  canData[0] = int(ccl*15);
  canData[0] += int(dcl*15) << 4;
  canData[1] += fault;
  canData[1] += estop << 1;

  CAN.beginPacket(CONTROL_CAN_ID);
  CAN.write(canData,8);
  CAN.endPacket();

}