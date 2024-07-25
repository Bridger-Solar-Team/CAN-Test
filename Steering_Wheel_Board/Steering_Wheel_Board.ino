#include <CAN.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define RIGHT_TURN_PIN 8
#define LEFT_TURN_PIN 9

bool leftTurn;
bool rightTurn;
uint8_t canData[8];

void setup() {
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3);
  pinMode(RIGHT_TURN_PIN, INPUT);
  pinMode(LEFT_TURN_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  writeCAN();
}

void readInputs() {
  leftTurn = digitalRead(LEFT_TURN_PIN);
  rightTurn = digitalRead(RIGHT_TURN_PIN);
}

void writeCAN() {
  canData[0] += leftTurn;
  canData[0] += rightTurn << 1;

  CAN.beginPacket(0x12);
  CAN.write(canData,8);
  CAN.endPacket();

}