#include <CAN.h>
#include <LiquidCrystal_I2C.h>

#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4
#define CANID 25

#define PIN_0 25
#define PIN_1 26
#define PIN_2 27
#define PIN_3 32
#define PIN_4 14
#define PIN_5 33

//Keep track of CAN receiving messages
char canData[8];
int dataID;
bool newCanData;

//Store pin state data
bool pins[6];
int pinsCanSpacing = 1000; //Time between CAN frames in milliseconds
unsigned long pinsCanTime = 0;

//Used for LCD CAN message printing
bool row = false;

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
  pinMode(PIN_0, INPUT_PULLDOWN);
  pinMode(PIN_1, INPUT_PULLDOWN);
  pinMode(PIN_2, INPUT_PULLDOWN);
  pinMode(PIN_3, INPUT_PULLDOWN);
  pinMode(PIN_4, INPUT_PULLDOWN);
  pinMode(PIN_5, INPUT_PULLDOWN);
}

void loop() {
  updatePins();

  if(millis() - pinsCanTime > pinsCanSpacing) {
    pinsCAN();
    pinsCanTime = millis();
  }

  serialCAN();

  if (newCanData) {
    printCAN();
    lcdCAN();
    newCanData = false;
  }
}

void pinsCAN() {
  CAN.beginPacket(CANID + 1);
  for (int i = 0; i < 6; i++) {
    CAN.write(48 + pins[i]);
  }
  CAN.endPacket();
  Serial.print("Pins written at ");
  Serial.println(millis());
}

void updatePins() {
  pins[0] = digitalRead(PIN_0);
  pins[1] = digitalRead(PIN_1);
  pins[2] = digitalRead(PIN_2);
  pins[3] = digitalRead(PIN_3);
  pins[4] = digitalRead(PIN_4);
  pins[5] = digitalRead(PIN_5);
}

void lcdCAN() {
  lcd.setCursor(row*1, 0);
  lcd.print(dataID);
  lcd.print(":");
  lcd.print(canData);
  row = !row;
}

void printCAN() {
  Serial.print(dataID);
  Serial.print(": ");
  for (int i = 0; i < 8; i++) {
    Serial.print(canData[i]);
  }
  Serial.println();
}

void serialCAN() {
  if (Serial.available() > 0) {
    CAN.beginPacket(CANID);
    for (int i = 0; i < 8; i++) {
      if (Serial.available() > 0) {
        CAN.write(Serial.read());
      }
    }
    CAN.endPacket();
    Serial.print("serialCAN written at ");
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
