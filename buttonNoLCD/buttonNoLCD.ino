#include <CAN.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define CANID 25

#define PIN_0 25 //Button pin

#define PIN_1 2 //Internal LED pin

//Keep track of CAN receiving messages
char canData[8];
int dataID;
bool newCanData;

//Store pin state data
bool pins[6];
int pinsCanSpacing = 50; //Time between CAN frames in milliseconds
unsigned long pinsCanTime = 0;

void setup() {
  //CAN setup
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Supported speeds are 125E3, 250E3, 500E3, 1000E3
  CAN.onReceive(readCAN);

  //Serial setup
  Serial.begin(115200);
  Serial.println("New Program");

  //Pins setup
  pinMode(PIN_0, INPUT_PULLDOWN);
  pinMode(PIN_1, OUTPUT);
}

void loop() {
  updatePins();

  if(millis() - pinsCanTime > pinsCanSpacing) {
    pinsCAN();
    pinsCanTime = millis();
  }

  if (newCanData) {
    printCAN();
    ledCAN();
    newCanData = false;
  }
}

void ledCAN() {
  digitalWrite(PIN_1, canData[0]);
}

void pinsCAN() {
  CAN.beginPacket(CANID + 1);
  for (int i = 0; i < 6; i++) {
    CAN.write(pins[i]);
  }
  CAN.endPacket();
  Serial.print("Pins written at ");
  Serial.println(millis());
}

void updatePins() {
  pins[0] = digitalRead(PIN_0);
}

void printCAN() {
  Serial.print(dataID);
  Serial.print(": ");
  for (int i = 0; i < 8; i++) {
    Serial.print(canData[i]);
  }
  Serial.println();
}

void readCAN(int packetSize) {
  dataID = CAN.packetId();
  for (int i = 0; i < 8; i++) {
    if (CAN.available()) {
      canData[i] = CAN.read();
    } else {
      canData[i] = 0;
    }
  }
  newCanData = true;
}
