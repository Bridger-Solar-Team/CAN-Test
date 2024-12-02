//Libraries
#include <CAN.h>

//Board Specific Contants
double STEERING_CAN_ID = 0b01011 << 24 && 0b00001000 << 16 && 12;
int canWriteSpacing = 50; //Time between CAN frames in milliseconds
unsigned long canWriteTime = 0;

//Pin Locations for Board
#define TX_GPIO_NUM 5
#define RX_GPIO_NUM 4

//Pin Locations for GPIO
#define HV_POWER_PIN 20
#define LEFT_TURN_PIN 21
#define RIGHT_TURN_PIN 22
#define HORN_PIN 23
#define FORWARD_REVERSE_PIN 24
#define DISPLAY_TOGGLE_PIN 25
#define HAZZARDS_PIN 26
#define CRUISE_CONTROL_PIN 27
#define THROTTLE_PIN 28

//Variables
double accel = 0;
bool newCanData = false;
double dataID;

//Arrays
byte canData[4][8];
byte canMessage[8];
bool pins[8];

void setup() {
  CAN.setPins(RX_GPIO_NUM, TX_GPIO_NUM);
  CAN.begin(1000E3);
  CAN.onReceive(readCAN);

  pinMode(HV_POWER_PIN, INPUT_PULLDOWN);
  pinMode(LEFT_TURN_PIN, INPUT_PULLDOWN);
  pinMode(RIGHT_TURN_PIN, INPUT_PULLDOWN);
  pinMode(HORN_PIN, INPUT_PULLDOWN);
  pinMode(FORWARD_REVERSE_PIN, INPUT_PULLDOWN);
  pinMode(DISPLAY_TOGGLE_PIN, INPUT_PULLDOWN);
  pinMode(HAZZARDS_PIN, INPUT_PULLDOWN);
  pinMode(CRUISE_CONTROL_PIN, INPUT_PULLDOWN);
}

void loop() {
  // put your main code here, to run repeatedly:
  readInputs();

  if(millis() - canWriteTime > canWriteSpacing) {
    writeCAN();
    canWriteTime = millis();
  }

}

void readInputs() {
  pins[7] = digitalRead(HV_POWER_PIN);
  pins[6] = digitalRead(LEFT_TURN_PIN);
  pins[5] = digitalRead(RIGHT_TURN_PIN);
  pins[4] = digitalRead(HORN_PIN);
  pins[3] = digitalRead(FORWARD_REVERSE_PIN);
  pins[2] = digitalRead(DISPLAY_TOGGLE_PIN);
  pins[1] = digitalRead(HAZZARDS_PIN);
  pins[0] = digitalRead(CRUISE_CONTROL_PIN);

  accel = analogRead(THROTTLE_PIN)/4095.0;
}

void readCAN(int packetSize) {
  //Tell code wehre to write CAN data
  dataID = CAN.packetId();
  int j = 0;
  if(dataID == 0x0B080019) {
    j = 1;
  }

  //Write and store CAN data
  for (int i = 0; i < 8; i++) {
    if (CAN.available()) {
      canData[j][i] = CAN.read();
    } else {
      canData[j][i] = 0;
    }
  }

  //If anything acts on new CAN data
  newCanData = true;
}

void writeCAN() {
  //Prep message array
  for(int i  = 0; i < 8; i++) {
    canMessage[0] += pins[i] << i;
  }

  canMessage[1] = accel*255.0;

  //Send message
  CAN.beginPacket(STEERING_CAN_ID);
  CAN.write(canMessage,8);
  CAN.endPacket();

}