//API reference: https://github.com/sandeepmistry/arduino-CAN/blob/master/API.md

#include <CAN.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4

bool leftTurn;
byte steeringData[8];

void setup() {
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3);
  CAN.onReceive(readData);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void readData(int packetSize) {
  int dataID = CAN.packetId();
  int position = 0;
  if(dataID == 0x20) {
    while(CAN.available()) {
      steeringData[position] = CAN.read();
      position += 1;
    }
  }
}