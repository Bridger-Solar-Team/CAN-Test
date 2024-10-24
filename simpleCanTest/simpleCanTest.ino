#include <CAN.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define CANID 25

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
}

void loop() {
  // if(sender){
  //   writeCAN();
  // }
  // if(!sender && newCanData){
  //   printCAN();
  //   newCanData = false;
  // }
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
