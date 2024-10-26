#include <CAN.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define CANID 25

#define PIN_1 25
#define PIN_2 26
#define PIN_3 27
#define PIN_4 32
#define PIN_5 34 //Input only
#define PIN_6 35 //Input only

char canData[8];
int dataID;
bool newCanData;

//Assign one board as the sender and one(or multiple) not as the sender
bool sender = false;

void setup() {
  //CAN setup
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3);
  CAN.onReceive(readCAN);

  //Serial setup
  Serial.begin(115200);
  Serial.println("New Program");

  //Pins setup
  pinMode(PIN_1, INPUT_PULLDOWN);
  pinMode(PIN_2, INPUT_PULLDOWN);
  pinMode(PIN_3, INPUT_PULLDOWN);
  pinMode(PIN_4, INPUT_PULLDOWN);
  pinMode(PIN_5, INPUT_PULLDOWN);
  pinMode(PIN_6, INPUT_PULLDOWN);
}

void loop() {
  writeCAN();
  if(newCanData) {
    printCAN();
    newCanData = false;
  }
}

void printCAN() {
    Serial.print(dataID);
    Serial.print(": ");
    for(int i = 0; i < 8; i++) {
      Serial.print(canData[i]);
    }
    Serial.println();
}

void writeCAN() {
  if(Serial.available() > 0){
    CAN.beginPacket(CANID);
    for(int i = 0; i < 8; i++){
      if(Serial.available() > 0){
        CAN.write(Serial.read());
      }
    }
    CAN.endPacket();
    Serial.print("CAN written at ");
    Serial.println(millis());
  }
}

void readCAN(int packetSize) {
  dataID = CAN.packetId();
  for(int i = 0; i < 8; i++) {
    if(CAN.available()) {
      canData[i] = CAN.read();
    } else {
      canData[i] = 0;
    }
  }
  newCanData = true;
}
