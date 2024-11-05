#include <CAN.h>
#include <LiquidCrystal_I2C.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define CANID 25

#define PIN_1 25
#define PIN_2 26
#define PIN_3 27
#define PIN_4 32
#define PIN_5 14
#define PIN_6 33

char canData[8];
int dataID;
bool newCanData;
bool pins[6];

//Assign one board as the sender and one(or multiple) not as the sender
bool sender = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  //CAN setup
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(500E3);
  CAN.onReceive(readCAN);

  //Serial setup
  Serial.begin(115200);
  Serial.println("New Program");

  //LCD Setup
  lcd.init();
  lcd.backlight();

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
  if (newCanData) {
    printCAN();
    lcdCAN();
    newCanData = false;
  }
}

bool buttonChanged() {
  if (digitalRead(PIN_1) != pins[0]) {
    pins[0] = digitalRead(PIN_1);
    Serial.println("Pin 1: " + pins[0]);
    return true;
  } else if (digitalRead(PIN_2) != pins[1]) {
    pins[1] = digitalRead(PIN_2);
    Serial.println("Pin 2: " + pins[1]);
    return true;
  }
  return false;
}

void lcdCAN() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(dataID);
  lcd.print(":");
  lcd.print(canData);
}

void printCAN() {
  Serial.print(dataID);
  Serial.print(": ");
  for (int i = 0; i < 8; i++) {
    Serial.print(canData[i]);
  }
  Serial.println();
}

void writeCAN() {
  if (Serial.available() > 0) {
    CAN.beginPacket(CANID);
    for (int i = 0; i < 8; i++) {
      if (Serial.available() > 0) {
        CAN.write(Serial.read());
      }
    }
    CAN.endPacket();
    Serial.print("CANboard written at ");
    Serial.println(millis());
  } 
  else if (buttonChanged()) {
    CAN.beginPacket(CANID + 1);
    for (int i = 0; i < 6; i++) {
      CAN.write(48 + pins[i]);
    }
    CAN.endPacket();
    Serial.print("Pins written at ");
    Serial.println(millis());
  }
}

void readCAN(int packetSize) {
  dataID = CAN.packetId();
  for (int i = 0; i < 8; i++) {
    if (CAN.available()) {
      canData[i] = CAN.read();
    } else {
      canData[i] = 32;
    }
  }
  newCanData = true;
}
