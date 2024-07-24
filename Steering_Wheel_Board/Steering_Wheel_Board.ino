#include <CAN.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4


void setup() {
  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3);
}

void loop() {
  // put your main code here, to run repeatedly:
  CAN.beginPacket(0x12);
  CAN.write('a');
  CAN.endPacket();
}