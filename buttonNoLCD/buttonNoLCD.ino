#include <CAN.h>

#define TX_GPIO_NUM 16
#define RX_GPIO_NUM 17

#define DASH_CANID 12
#define BATTERYBOX_CANID 25
#define LOGGER_CANID 14
#define BMS_CANID 30

//Keep track of CAN receiving messages
char canData[5][8];
int dataID;
bool newCanData;
int canArray = 0;

void setup() {
  //CAN setup
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3); // Supported speeds are 125E3, 250E3, 500E3, 1000E3
  CAN.onReceive(readCAN);

  //Serial setup
  Serial.begin(115200);
  Serial.println("New Program");
}

void loop() {
  if (newCanData) {
    printCAN();
    // ledCAN();
    newCanData = false;
  }
}

void printCAN() {
  Serial.print(dataID);
  Serial.print(": ");
  for (int i = 0; i < 8; i++) {
    Serial.print(canData[canArray][i]);
  }
  Serial.println();
}

void readCAN(int packetSize) {
  dataID = CAN.packetId();

  canArray = 0;
  switch(dataID) {
    //Dashboard
    case DASH_CANID:
      canArray = 1;
      break;
    //Battery Box Board
    case BATTERYBOX_CANID:
      canArray = 2;
      break;
    //SD Data Logging Board
    case LOGGER_CANID:
      canArray = 3;
      break;
    //BMS Messages
    case BMS_CANID:
      canArray = 4;
      break;
    default: 
      canArray = 0;
      break;
  }

  for (int i = 0; i < 8; i++) {
    if (CAN.available()) {
      canData[canArray][i] = CAN.read();
    } else {
      canData[canArray][i] = 0;
    }
  }
  newCanData = true;
}
