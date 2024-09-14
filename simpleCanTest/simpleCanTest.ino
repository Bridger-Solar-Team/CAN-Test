#include <CAN.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define CANID 25

int canData[8];
int dataID;
bool newCanData;

//Assign one board as the sender and one(or multiple) not as the sender
bool sender = true;

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
  if(sender){
    writeCAN();
    Serial.print("CAN written at");
    Serial.println(millis());
  }
  if(!sender && newCanData){
    Serial.print("ID: ");
    Serial.println(dataID);
    for(int i = 0; i < 8; i++) {
      Serial.print("Byte ");
      Serial.print(i);
      Serial.print(" : ");
      Serial.println(canData[i]);
    }
    Serial.println();
    newCanData = false;
  }
}

void writeCAN() {
  CAN.beginPacket(CANID);
  CAN.write(0x01);
  CAN.write(0x02);
  CAN.write(0x04);
  CAN.write(0x08);
  CAN.write(0x10);
  CAN.write(0x20);
  CAN.write(0x40);
  CAN.write(0x80);
  CAN.endPacket();
}

void readCAN(int packetSize) {
  dataID = CAN.packetId();
  int position = 0;
  while(CAN.available()) {
    canData[position] = CAN.read();
    position += 1;
  }
  newCanData = true;
}
